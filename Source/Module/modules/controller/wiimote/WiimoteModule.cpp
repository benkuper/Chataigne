/*
  ==============================================================================

	WiimoteModule.cpp
	Created: 26 Dec 2016 6:20:01pm
	Author:  Ben

  ==============================================================================
*/


WiimoteModule::WiimoteModule(const String & name) :
	Module(name),
    irCC("IR")
{
	setupIOConfiguration(true, false);

	deviceID = moduleParams.addIntParameter("Device Select", "Select the ID of the device, depending on how many wiimotes are connected (max 4)", 1, 1, 4);
	connectTrigger = moduleParams.addTrigger("Refresh Wiimotes", "Try to reconnect the wiimotes");
	smoothing = moduleParams.addFloatParameter("Smoothing", "Set the orientation smoothing for the wiimote orientation algorithm", .5f, 0, 1);

	connected = valuesCC.addBoolParameter("Connected", "Wiimote is connected ?", false);

	nunchuckConnected = valuesCC.addBoolParameter("Nunchuck", "Nunchuck is connected ? ", false);
	motionPlusConnected = valuesCC.addBoolParameter("MotionPlus", "MotionPlus is connected ? ", false);
	batteryLevel = valuesCC.addFloatParameter("Battery", "Battery level of the wiimote", 0, 0, 1);
	pitch = valuesCC.addFloatParameter("Pitch", "Pitch of the wiimote", 0, -1, 1);
	roll = valuesCC.addFloatParameter("Roll", "Roll of the wiimote", 0, -1, 1);
	yaw = valuesCC.addFloatParameter("Yaw", "Yaw of the wiimote.\nMotion Plus must be connected !", 0, -1, 1);

	nunchuckX = valuesCC.addFloatParameter("Nunchuck X", "X axis of the nunchuck if plugged",0,-1,1);
	nunchuckY = valuesCC.addFloatParameter("Nunchuck Y", "Y axis of the nunchuck if plugged",0,-1,1);

	accelX = valuesCC.addFloatParameter("Accel X", "Raw values of accelerometer",0,-1,1);
	accelY = valuesCC.addFloatParameter("Accel Y", "Raw values of accelerometer", 0, -1, 1);
	accelZ = valuesCC.addFloatParameter("Accel Z", "Raw values of accelerometer", 0, -1, 1);
	rawAccelX = valuesCC.addFloatParameter("Raw Accel X", "Raw values of accelerometer", 0, 0,300);
	rawAccelY = valuesCC.addFloatParameter("Raw Accel Y", "Raw values of accelerometer", 0, 0,300);
	rawAccelZ = valuesCC.addFloatParameter("Raw Accel Z", "Raw values of accelerometer", 0, 0,300);

	

	
	BoolParameter * bt_a = valuesCC.addBoolParameter("Button A", "Button A", false);
	BoolParameter * bt_b = valuesCC.addBoolParameter("Button B", "Button B", false);
	BoolParameter * bt_c = valuesCC.addBoolParameter("Button C", "Button C", false);
	BoolParameter * bt_z = valuesCC.addBoolParameter("Button Z", "Button Z", false);
	BoolParameter * bt_1 = valuesCC.addBoolParameter("Button 1", "Button 1", false);
	BoolParameter * bt_2 = valuesCC.addBoolParameter("Button 2", "Button 2", false);
	BoolParameter * bt_h = valuesCC.addBoolParameter("Button Home", "Button Home", false);
	BoolParameter * bt_mi = valuesCC.addBoolParameter("Button -", "Button -", false);
	bt_mi->setCustomShortName("buttonMinus");
	BoolParameter * bt_pl = valuesCC.addBoolParameter("Button +", "Button +", false);
	bt_pl->setCustomShortName("buttonPlus"); //avoid autoShortName to mix up + and - as they're both converted to _ (needs fixing?)
	BoolParameter * bt_up = valuesCC.addBoolParameter("Button Up", "Button Up", false);
	BoolParameter * bt_do = valuesCC.addBoolParameter("Button Down", "Button Down", false);
	BoolParameter * bt_le = valuesCC.addBoolParameter("Button Left", "Button Left", false);
	BoolParameter * bt_ri = valuesCC.addBoolParameter("Button Right", "Button Right", false);


	buttons.addArray({ bt_2,bt_1,bt_b,bt_a,bt_mi,bt_c,bt_z,bt_h,bt_le,bt_ri,bt_do,bt_up,bt_pl });

	irNumPoints = irCC.addIntParameter("Num Points", "Number of detected Infrared sources by the wiimote", 0, 0, 4);
	irDistance = irCC.addFloatParameter("Point Distance", "Distance detected by the first 2 points", 0, 0);
	irPos = irCC.addPoint3DParameter("Position", "Estimated 3D position from the detected points");
	for (int i = 0; i < 4; i++)
	{
		Point2DParameter* p2d = irCC.addPoint2DParameter("Point " + String(i + 1), "Normalized -1/1 position of this point");
		p2d->setBounds(-1, -1, 1, 1);
		irPoints.add(p2d);
	}

	valuesCC.addChildControllableContainer(&irCC);

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	connectionFeedbackRef = connected;

	WiimoteManager::getInstance();
	startTimerHz(100);
}

WiimoteModule::~WiimoteModule()
{
    stopTimer();
}

void WiimoteModule::timerCallback()
{
    if (isClearing) return;
    auto* manager = WiimoteManager::getInstanceWithoutCreating();
    if (manager == nullptr) return;
    auto state = manager->getState(deviceID->intValue() - 1);
    if (!enabled->boolValue()) state = {};
    // A mapping triggered by a value can delete this module synchronously.
    const WeakReference<ControllableContainer> alive(this);
    const auto set = [&alive](Parameter* parameter, const var& value)
    {
        parameter->setValue(value);
        return alive.get() != nullptr;
    };
    if (!set(connected, state.connected)) return;
    if (!set(nunchuckConnected, state.nunchuk)) return;
    if (!set(motionPlusConnected, state.motionPlus)) return;
    if (!set(batteryLevel, state.battery)) return;
    for (int i = 0; i < NUM_WIIMOTE_BUTTONS; ++i)
        if (!set(buttons[i], state.buttons[(size_t) i])) return;
    if (!set(pitch, state.pitch) || !set(roll, state.roll) || !set(yaw, state.yaw)) return;
    if (!set(accelX, state.accelX) || !set(accelY, state.accelY) || !set(accelZ, state.accelZ)) return;
    if (!set(rawAccelX, state.rawAccelX) || !set(rawAccelY, state.rawAccelY) || !set(rawAccelZ, state.rawAccelZ)) return;
    if (!set(nunchuckX, state.joystickX) || !set(nunchuckY, state.joystickY)) return;
    if (!set(irNumPoints, state.irNumDots) || !set(irDistance, state.irDistance)) return;
    irPos->setVector(state.irPos);
    if (alive.get() == nullptr) return;
    for (int i = 0; i < 4; ++i)
    {
        irPoints[i]->setPoint(state.irPoints[(size_t) i]);
        if (alive.get() == nullptr) return;
    }
}

void WiimoteModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
    Module::onControllableFeedbackUpdateInternal(cc, c);
    if (c == smoothing || c == deviceID)
        WiimoteManager::getInstance()->setSmoothing(deviceID->intValue() - 1, 1.f - smoothing->floatValue() * .9f);
    else if (c == connectTrigger)
        WiimoteManager::getInstance()->requestReconnect();
    else if (c != nullptr && (c->parentContainer == &valuesCC || c->parentContainer == &irCC))
        inActivityTrigger->trigger();
}
