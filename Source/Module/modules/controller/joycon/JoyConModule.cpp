/*
  ==============================================================================

	JoyConModule.cpp
	Created: 23 Jan 2018 4:30:24pm
	Author:  Ben

  ==============================================================================
*/

JoyConModule::JoyConModule() :
	Module(getDefaultTypeString()),
	leftValues("Left Controller"),
	rightValues("Right Controller")
{

	valuesCC.addChildControllableContainer(&leftValues);
	valuesCC.addChildControllableContainer(&rightValues);

	reconnectControllers = moduleParams.addTrigger("Reconnect Controllers", "Retry controller discovery. Pair original Joy-Cons in OS Bluetooth settings. For Joy-Con 2 on Windows 10/11, hold each controller's SYNC button until its LEDs cycle; Chataigne connects directly over Bluetooth LE.");

	useJitterThreshold = moduleParams.addBoolParameter("Use Jitter Threshold", "Only send accel/orentiation/stick changes if the delta is greater than the jitter threshold.", true);
	jitterThreshold = moduleParams.addFloatParameter("Jitter Threshold", "Delta change threshold for analog inputs. Only update values if the change is greater than the threshold.", 0.005, 0.0, 0.1, true);

	leftAccel = leftValues.addPoint3DParameter("Left Accel", "");
	leftAccel->setBounds(-1, -1, -1, 1, 1, 1);
	leftOrientation = leftValues.addPoint3DParameter("Left Orientation", "");
	leftOrientation->setBounds(-1, -1, -1, 1, 1, 1);
	
	leftAxis = leftValues.addPoint2DParameter("Left Axis", "");
	leftAxis->setBounds(-1, -1, 1, 1);
	lStick = leftValues.addBoolParameter("Left Stick", "", false);
	up = leftValues.addBoolParameter("Up", "", false);
	down = leftValues.addBoolParameter("Down", "", false);
	left = leftValues.addBoolParameter("Left", "", false);
	right = leftValues.addBoolParameter("Right", "", false);
	l = leftValues.addBoolParameter("L", "", false);
	zl = leftValues.addBoolParameter("ZL", "", false);
	leftSL = leftValues.addBoolParameter("Left SL", "", false);
	leftSR = leftValues.addBoolParameter("Left SR", "", false);
	minus = leftValues.addBoolParameter("-", "", false);
	capture = leftValues.addBoolParameter("Capture", "", false);

	rightAccel = rightValues.addPoint3DParameter("Right Accel", "");
	rightAccel->setBounds(-1, -1, -1, 1, 1, 1);
	rightOrientation = rightValues.addPoint3DParameter("Right Orientation", "");
	rightOrientation->setBounds(-1, -1, -1, 1, 1, 1);
	
	rightAxis = rightValues.addPoint2DParameter("Right Axis", "");
	rightAxis->setBounds(-1, -1, 1, 1);
	leftConnected = leftValues.addBoolParameter("Connected", "Whether a left controller is sending input.", false);
	rightConnected = rightValues.addBoolParameter("Connected", "Whether a right controller is sending input.", false);
	leftConnected->isControllableFeedbackOnly = true;
	rightConnected->isControllableFeedbackOnly = true;
	cButton = rightValues.addBoolParameter("C", "Joy-Con 2 C button.", false);
	rStick = rightValues.addBoolParameter("Right Stick", "", false);
	a = rightValues.addBoolParameter("A", "", false);
	b = rightValues.addBoolParameter("B", "", false);
	x = rightValues.addBoolParameter("X", "", false);
	y = rightValues.addBoolParameter("Y", "", false);
	r = rightValues.addBoolParameter("R", "", false);
	zr = rightValues.addBoolParameter("ZR", "", false);
	rightSL = rightValues.addBoolParameter("Right SL", "", false);
	rightSR = rightValues.addBoolParameter("Right SR", "", false);
	plus = rightValues.addBoolParameter("+", "", false);
	home = rightValues.addBoolParameter("Home", "", false);

	//for (auto &c : leftValues.controllables) c->isControllableFeedbackOnly = true;
	//for (auto &c : rightValues.controllables) c->isControllableFeedbackOnly = true;


	controllers = std::make_unique<JoyConDeviceSession>();
	startTimerHz(100);
}

JoyConModule::~JoyConModule()
{
    stopTimer();
    controllers.reset();
}

void JoyConModule::timerCallback()
{
    if (isClearing || controllers == nullptr) return;
    auto frames = controllers->getFrames();
    if (!enabled->boolValue()) frames = {};
    const WeakReference<ControllableContainer> alive(this);
    leftConnected->setValue(frames[0].connected);
    if (alive.get() == nullptr) return;
    rightConnected->setValue(frames[1].connected);
    if (alive.get() == nullptr) return;
    cButton->setValue(frames[1].connected && frames[1].c);
    if (alive.get() == nullptr) return;
    const bool suppress = useJitterThreshold->boolValue();
    const float threshold = jitterThreshold->floatValue();
    const std::array<std::array<BoolParameter*, 11>, 2> buttons {{
        {{ up, down, left, right, minus, lStick, l, zl, capture, leftSL, leftSR }},
        {{ plus, rStick, r, zr, b, a, y, x, home, rightSL, rightSR }}
    }};
    const int bits[2][11] = {{0,1,2,3,5,6,8,10,17,18,19}, {4,7,9,11,12,13,14,15,16,18,19}};
    const std::array<Point3DParameter*, 2> accels {{ leftAccel, rightAccel }};
    const std::array<Point3DParameter*, 2> orientations {{ leftOrientation, rightOrientation }};
    const std::array<Point2DParameter*, 2> axes {{ leftAxis, rightAxis }};
    const auto finite = [](float value) { return std::isfinite(value) ? value : 0.f; };
    const auto changed = [suppress, threshold](float x, float y, float z, float a, float b, float c)
    {
        return !suppress || std::abs(x-a) > threshold || std::abs(y-b) > threshold || std::abs(z-c) > threshold;
    };
    for (size_t side = 0; side < 2; ++side)
    {
        auto frame = frames[side];
        if (!frame.connected) frame = {};
        for (int i = 0; i < 11; ++i)
        {
            buttons[side][(size_t) i]->setValue((frame.state.buttons & (1 << bits[side][i])) != 0);
            if (alive.get() == nullptr) return;
        }
        auto& last = lastFrames[side];
        const auto& motion = frame.motion;
        const bool force = !frame.connected || !last.connected;
        const float ax = finite(motion.accelX), ay = finite(motion.accelY), az = finite(motion.accelZ);
        if (force || changed(ax, ay, az, last.motion.accelX, last.motion.accelY, last.motion.accelZ))
        {
            last.motion.accelX = ax; last.motion.accelY = ay; last.motion.accelZ = az;
            accels[side]->setVector(ax, ay, az);
            if (alive.get() == nullptr) return;
        }
        const float gx = finite(motion.gravX), gy = finite(motion.gravY), gz = finite(motion.gravZ);
        if (force || changed(gx, gy, gz, last.motion.gravX, last.motion.gravY, last.motion.gravZ))
        {
            last.motion.gravX = gx; last.motion.gravY = gy; last.motion.gravZ = gz;
            orientations[side]->setVector(gx, gy, gz);
            if (alive.get() == nullptr) return;
        }
        const float sx = finite(side == 0 ? frame.state.stickLX : frame.state.stickRX);
        const float sy = finite(side == 0 ? frame.state.stickLY : frame.state.stickRY);
        if (force || changed(sx, sy, 0, last.state.stickLX, last.state.stickLY, 0))
        {
            last.state.stickLX = sx; last.state.stickLY = sy;
            axes[side]->setPoint(sx, sy);
            if (alive.get() == nullptr) return;
        }
        last.connected = frame.connected;
    }
}

void JoyConModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
    const WeakReference<ControllableContainer> alive(this);
    Module::onControllableFeedbackUpdateInternal(cc, c);
    if (alive.get() == nullptr) return;
    if (c == reconnectControllers)
    {
        if (controllers != nullptr) controllers->reconnect();
    }
    else if (c != nullptr && (c->parentContainer == &leftValues || c->parentContainer == &rightValues))
        inActivityTrigger->trigger();
}
