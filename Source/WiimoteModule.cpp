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

	connected = valuesCC.addBoolParameter("Connected", "Wiimote is connected ?", false);
	connected->isControllableFeedbackOnly = true;
	nunchuckConnected = valuesCC.addBoolParameter("Nunchuck", "Nunchuck is connected ? ", false);
	nunchuckConnected->isControllableFeedbackOnly = true;
	motionPlusConnected = valuesCC.addBoolParameter("MotionPlus", "MotionPlus is connected ? ", false);
	motionPlusConnected->isControllableFeedbackOnly = true;
	batteryLevel = valuesCC.addFloatParameter("Battery", "Battery level of the wiimote", 0,0,1);
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

	buttons.add(valuesCC.addBoolParameter("Button A", "Button A", false));
	buttons.add(valuesCC.addBoolParameter("Button B", "Button B", false));
	buttons.add(valuesCC.addBoolParameter("Button C", "Button C", false));
	buttons.add(valuesCC.addBoolParameter("Button Z", "Button Z", false));
	buttons.add(valuesCC.addBoolParameter("Button 1", "Button 1", false));
	buttons.add(valuesCC.addBoolParameter("Button 2", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button Home", "Button Home", false));
	buttons.add(valuesCC.addBoolParameter("Button -", "Button -", false));
	buttons.add(valuesCC.addBoolParameter("Button +", "Button +", false));
	buttons.add(valuesCC.addBoolParameter("Button Up", "Button Up", false));
	buttons.add(valuesCC.addBoolParameter("Button Down", "Button Down", false));
	buttons.add(valuesCC.addBoolParameter("Button Left", "Button Left", false));
	buttons.add(valuesCC.addBoolParameter("Button Right", "Button Right", false));

	for (auto &p : buttons) p->isControllableFeedbackOnly = true;
	

	WiimoteManager::getInstance()->addListener(this);
}

WiimoteModule::~WiimoteModule()
{
	if(WiimoteManager::getInstanceWithoutCreating() != nullptr) WiimoteManager::getInstance()->removeListener(this);
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
	float targetPitch = -device->pitch;
	pitch->setValue(pitch->floatValue()+(targetPitch - pitch->floatValue())*.5f);

	float r = -acosf(device->roll);
	if (device->yaw < 0) r = -r;
	r += float_Pi / 2;
	r /= float_Pi;
	if (r > 1) r -= 2;

	if (!std::isnan(r))
	{
		if (std::abs(r - roll->floatValue()) > .5f) roll->setValue(r);
		else roll->setValue(roll->floatValue() + (r - roll->floatValue())*.5f);
	}

	yaw->setValue(device->yaw/float_Pi);
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
