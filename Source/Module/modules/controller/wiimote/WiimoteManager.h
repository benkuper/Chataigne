#pragma once

#include "wiiuse.h"
#include <array>
#include <atomic>

#define MAX_WIIMOTES 4
#define NUM_WIIMOTE_BUTTONS 13

// Copies only: native WiiUse objects never leave the hardware thread.
struct WiimoteState
{
    bool connected = false, nunchuk = false, motionPlus = false;
    std::array<bool, NUM_WIIMOTE_BUTTONS> buttons {};
    float battery = 0, yaw = 0, pitch = 0, roll = 0;
    float accelX = 0, accelY = 0, accelZ = 0;
    float rawAccelX = 0, rawAccelY = 0, rawAccelZ = 0;
    float joystickX = 0, joystickY = 0, irDistance = 0;
    int irNumDots = 0;
    Vector3D<float> irPos {};
    std::array<Point<float>, 4> irPoints {};
};

class WiimoteManager : public Thread
{
public:
    juce_DeclareSingleton(WiimoteManager, false);
    WiimoteManager();
    ~WiimoteManager();

    WiimoteState getState(int index);
    void requestReconnect();
    void setSmoothing(int index, float value);
    void run() override;

private:
    wiimote** devices = nullptr; // owned, accessed and destroyed by run()
    CriticalSection stateLock;
    std::array<WiimoteState, MAX_WIIMOTES> states {};
    std::array<std::atomic<float>, MAX_WIIMOTES> smoothing;
    std::atomic<bool> reconnectRequested { true }, pairRequested { false };
    void reconnect(bool allowPairing);
    void publishStates();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WiimoteManager)
};
