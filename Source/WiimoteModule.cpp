/*
  ==============================================================================

	WiimoteModule.cpp
	Created: 26 Dec 2016 6:20:01pm
	Author:  Ben

  ==============================================================================
*/


#include "WiimoteModule.h"

WiimoteModule::WiimoteModule(const String & name) :
	Module(name),
	device(nullptr)
{

	deviceID = addIntParameter("Device Select", "Select the ID of the device, depending on how many wiimotes are connected (max 4)", 1, 1, 4);
	connectTrigger = addTrigger("Refresh Wiimotes", "Try to reconnect the wiimotes");
	smoothing = addFloatParameter("Smoothing", "Set the orientation smoothing for the wiimote orientation algorithm", .5f, 0, 1);

	connected = valuesCC.addBoolParameter("Connected", "Wiimote is connected ?", false);
	connected->isControllableFeedbackOnly = true;

	nunchuckConnected = valuesCC.addBoolParameter("Nunchuck", "Nunchuck is connected ? ", false);
	nunchuckConnected->isControllableFeedbackOnly = true;
	motionPlusConnected = valuesCC.addBoolParameter("MotionPlus", "MotionPlus is connected ? ", false);
	motionPlusConnected->isControllableFeedbackOnly = true;
	batteryLevel = valuesCC.addFloatParameter("Battery", "Battery level of the wiimote", 0, 0, 1);
	batteryLevel->isControllableFeedbackOnly = true;
	pitch = valuesCC.addFloatParameter("Pitch", "Pitch of the wiimote", 0, -1, 1);
	pitch->isControllableFeedbackOnly = true;
	roll = valuesCC.addFloatParameter("Roll", "Roll of the wiimote", 0, -1, 1);
	roll->isControllableFeedbackOnly = true;
	yaw = valuesCC.addFloatParameter("Yaw", "Yaw of the wiimote.\nMotion Plus must be connected !", 0, -1, 1);
	yaw->isControllableFeedbackOnly = true;

	nunchuckXYAxis = valuesCC.addPoint2DParameter("Nunchuck XY", "X and Y axis of the nunchuck if plugged");
	nunchuckXYAxis->setBounds(-1, -1, 1, 1);
	nunchuckXYAxis->isControllableFeedbackOnly = true;
	nunchuckXYAxis->isEditable = false;

	accelParam = valuesCC.addPoint3DParameter("Raw Accel", "Raw values of accelerometer");
	accelParam->isControllableFeedbackOnly = true;

	BoolParameter * bt_a = valuesCC.addBoolParameter("Button A", "Button A", false);
	BoolParameter * bt_b = valuesCC.addBoolParameter("Button B", "Button B", false);
	BoolParameter * bt_c = valuesCC.addBoolParameter("Button C", "Button C", false);
	BoolParameter * bt_z = valuesCC.addBoolParameter("Button Z", "Button Z", false);
	BoolParameter * bt_1 = valuesCC.addBoolParameter("Button 1", "Button 1", false);
	BoolParameter * bt_2 = valuesCC.addBoolParameter("Button 2", "Button 2", false);
	BoolParameter * bt_h = valuesCC.addBoolParameter("Button Home", "Button Home", false);
	BoolParameter * bt_mi = valuesCC.addBoolParameter("Button -", "Button -", false);
	BoolParameter * bt_pl = valuesCC.addBoolParameter("Button +", "Button +", false);
	BoolParameter * bt_up = valuesCC.addBoolParameter("Button Up", "Button Up", false);
	BoolParameter * bt_do = valuesCC.addBoolParameter("Button Down", "Button Down", false);
	BoolParameter * bt_le = valuesCC.addBoolParameter("Button Left", "Button Left", false);
	BoolParameter * bt_ri = valuesCC.addBoolParameter("Button Right", "Button Right", false);


	buttons.addArray({ bt_2,bt_1,bt_b,bt_a,bt_mi,bt_c,bt_z,bt_h,bt_le,bt_ri,bt_do,bt_up,bt_pl });

	for (auto &p : buttons) p->isControllableFeedbackOnly = true;


	WiimoteManager::getInstance()->addListener(this);
}

WiimoteModule::~WiimoteModule()
{
	if (WiimoteManager::getInstanceWithoutCreating() != nullptr) WiimoteManager::getInstance()->removeListener(this);
	if (device != nullptr)
	{
		device->removeListener(this);
		device = nullptr;
	}
}

void WiimoteModule::deviceConnected(Wiimote * w)
{
	if (device == nullptr)
	{
		device = w;
		w->addListener(this);
		connected->setValue(true);
		smoothing->setValue(device->smoothing);
	}
}

void WiimoteModule::deviceDisconnected(Wiimote * w)
{
	if (device == w)
	{
		device->removeListener(this);
		device = nullptr;
		connected->setValue(false);
	}
}

void WiimoteModule::wiimoteButtonPressed(Wiimote * w, Wiimote::WiimoteButton b)
{
	if (w != device) return;
	buttons[(int)b]->setValue(true);
	inActivityTrigger->trigger();
}

void WiimoteModule::wiimoteButtonReleased(Wiimote * w, Wiimote::WiimoteButton b)
{
	if (w != device) return;
	buttons[(int)b]->setValue(false);
	inActivityTrigger->trigger();
}

void WiimoteModule::wiimoteOrientationUpdated(Wiimote * w)
{
	if (w != device) return;

	pitch->setValue(device->pitch);
	roll->setValue(device->roll);
	yaw->setValue(device->yaw);
}

void WiimoteModule::wiimoteAccelUpdated(Wiimote * w)
{
	if (w != device) return;
	accelParam->setVector(device->accelX, device->accelY, device->accelZ);
}

void WiimoteModule::wiimoteJoystickUpdated(Wiimote * w)
{
	if (w != device) return;
	nunchuckXYAxis->setPoint(device->joystickX, device->joystickY);
}

void WiimoteModule::wiimoteBatteryLevelChanged(Wiimote *)
{
	batteryLevel->setValue(device->batteryLevel);
}

void WiimoteModule::wiimoteNunchuckPlugged(Wiimote *)
{
	nunchuckConnected->setValue(true);
}

void WiimoteModule::wiimoteNunchuckUnplugged(Wiimote *)
{
	nunchuckConnected->setValue(false);
}

void WiimoteModule::wiimoteMotionPlusPlugged(Wiimote *)
{
	motionPlusConnected->setValue(true);
}

void WiimoteModule::wiimoteMotionPlusUnplugged(Wiimote *)
{
	motionPlusConnected->setValue(false);
}

void WiimoteModule::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == smoothing)
	{
		if (device != nullptr) device->setSmoothing(1 - (smoothing->floatValue()*.9f) + .1f); //force minimum .1f
	}
}

void WiimoteModule::onContainerTriggerTriggered(Trigger * t)
{
	if (t == connectTrigger)
	{
		WiimoteManager::getInstance()->reinitWiimotes = true;
	}
}
