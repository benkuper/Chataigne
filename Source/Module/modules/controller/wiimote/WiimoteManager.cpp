#if JUCE_MAC
extern "C" void wiiuse_os_cancel_discovery(void);
extern "C" void wiiuse_os_reset_discovery(void);
#endif

juce_ImplementSingleton(WiimoteManager)

WiimoteManager::WiimoteManager() : Thread("wiimoteManager")
{
    for (auto& value : smoothing) value.store(.5f);
    WiiPairUtil::shouldStop = false;
    startThread();
}

WiimoteManager::~WiimoteManager()
{
    signalThreadShouldExit();
    WiiPairUtil::shouldStop = true;
#if JUCE_MAC
    wiiuse_os_cancel_discovery();
#endif
    notify();
    stopThread(-1); // never kill a thread inside Bluetooth or WiiUse
    clearSingletonInstance();
}

WiimoteState WiimoteManager::getState(int index)
{
    const ScopedLock lock(stateLock);
    return isPositiveAndBelow(index, MAX_WIIMOTES) ? states[(size_t) index] : WiimoteState {};
}

void WiimoteManager::requestReconnect()
{
    pairRequested.store(true);
    reconnectRequested.store(true);
    notify();
}

void WiimoteManager::setSmoothing(int index, float value)
{
    if (isPositiveAndBelow(index, MAX_WIIMOTES)) smoothing[(size_t) index].store(jlimit(.1f, 1.f, value));
}

void WiimoteManager::reconnect(bool allowPairing)
{
    // Cleanup invalidates every native pointer, so always allocate a fresh array.
    wiiuse_cleanup(devices, MAX_WIIMOTES);
    devices = nullptr;
    {
        const ScopedLock lock(stateLock);
        states = {};
    }
#if JUCE_MAC
    wiiuse_os_reset_discovery();
#endif
    if (threadShouldExit()) return;
    devices = wiiuse_init(MAX_WIIMOTES);
    if (devices == nullptr) return;
    const int found = wiiuse_find(devices, MAX_WIIMOTES, 5);
    NLOG("Wiimote", "Found " << found << " Wii Remotes. Press 1+2 (or SYNC) while refreshing.");
    if (threadShouldExit()) return;
    int connected = wiiuse_connect(devices, MAX_WIIMOTES);
#if JUCE_WINDOWS
    if (connected == 0 && allowPairing && !threadShouldExit())
    {
        NLOG("Wiimote", "Press 1+2 (or SYNC) to pair a Wii Remote.");
        if (WiiPairUtil::pair() > 0 && !threadShouldExit())
        {
            wiiuse_cleanup(devices, MAX_WIIMOTES);
            devices = wiiuse_init(MAX_WIIMOTES);
            if (devices != nullptr)
            {
                wiiuse_find(devices, MAX_WIIMOTES, 5);
                if (!threadShouldExit()) connected = wiiuse_connect(devices, MAX_WIIMOTES);
            }
        }
    }
#else
    ignoreUnused(allowPairing);
#endif
    if (devices == nullptr) return;
    NLOG("Wiimote", "Connected to " << connected << " Wii Remotes");
    // Pairing on Windows is an explicit user action (Refresh), never an endless
    // automatic loop during module creation. Existing OS pairings are preserved.
    for (int i = 0; i < MAX_WIIMOTES && !threadShouldExit(); ++i)
    {
        auto* device = devices[i];
        if (device == nullptr || !WIIMOTE_IS_CONNECTED(device)) continue;
        wiiuse_motion_sensing(device, 1);
        wiiuse_set_flags(device, WIIUSE_SMOOTHING, 0);
        wiiuse_set_smooth_alpha(device, smoothing[(size_t) i].load());
        wiiuse_set_ir(device, 1);
        wiiuse_set_leds(device, WIIMOTE_LED_1 >> i);
    }
}

void WiimoteManager::publishStates()
{
    std::array<WiimoteState, MAX_WIIMOTES> next {};
    if (devices != nullptr)
        for (int i = 0; i < MAX_WIIMOTES; ++i)
        {
            const auto* d = devices[i];
            auto& s = next[(size_t) i];
            if (d == nullptr || !WIIMOTE_IS_CONNECTED(d)) continue;
            s.connected = true;
            s.nunchuk = d->exp.type == EXP_NUNCHUK || d->exp.type == EXP_MOTION_PLUS_NUNCHUK;
            s.motionPlus = d->exp.type == EXP_MOTION_PLUS || d->exp.type == EXP_MOTION_PLUS_NUNCHUK || d->exp.type == EXP_MOTION_PLUS_CLASSIC;
            for (int b = 0; b < NUM_WIIMOTE_BUTTONS; ++b) s.buttons[(size_t) b] = (d->btns & (1 << b)) != 0;
            s.buttons[5] = s.nunchuk && (d->exp.nunchuk.btns & NUNCHUK_BUTTON_C) != 0;
            s.buttons[6] = s.nunchuk && (d->exp.nunchuk.btns & NUNCHUK_BUTTON_Z) != 0;
            s.battery = d->battery_level;
            s.yaw = d->orient.yaw / 180.f;
            s.pitch = -d->orient.pitch / 90.f;
            s.roll = d->orient.roll / 180.f;
            s.accelX = d->gforce.x; s.accelY = d->gforce.y; s.accelZ = d->gforce.z;
            s.rawAccelX = d->accel.x; s.rawAccelY = d->accel.y; s.rawAccelZ = d->accel.z;
            if (s.nunchuk)
            {
                s.joystickX = d->exp.nunchuk.js.x;
                s.joystickY = d->exp.nunchuk.js.y;
            }
            if (d->ir.vres[0] > 0 && d->ir.vres[1] > 0)
            {
                // Dots occupy fixed slots; visible dots need not be contiguous.
                for (const auto& dot : d->ir.dot)
                    if (dot.visible && s.irNumDots < 4)
                        s.irPoints[(size_t) s.irNumDots++] = { (float) dot.x / d->ir.vres[0] * 2.f - 1.f, (float) dot.y / d->ir.vres[1] * 2.f - 1.f };
                s.irDistance = d->ir.distance;
                s.irPos = { (float) d->ir.x, (float) d->ir.y, d->ir.z };
            }
        }
    const ScopedLock lock(stateLock);
    states = next;
}

void WiimoteManager::run()
{
    while (!threadShouldExit())
    {
        if (reconnectRequested.exchange(false)) reconnect(pairRequested.exchange(false));
        if (threadShouldExit()) break;
        if (devices != nullptr)
        {
            for (int i = 0; i < MAX_WIIMOTES; ++i)
                if (devices[i] != nullptr && WIIMOTE_IS_CONNECTED(devices[i]))
                    wiiuse_set_smooth_alpha(devices[i], smoothing[(size_t) i].load());
            wiiuse_poll(devices, MAX_WIIMOTES);
        }
        publishStates();
        wait(5);
    }
    wiiuse_cleanup(devices, MAX_WIIMOTES);
    devices = nullptr;
}
