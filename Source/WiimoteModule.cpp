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
	connected = valuesCC.addBoolParameter("Connected", "Wiimote is connected ?", false);
	pitch = valuesCC.addFloatParameter("Pitch", "Pitch of the wiimote", 0, -1, 1);
	roll = valuesCC.addFloatParameter("Roll", "Roll of the wiimote", 0, -1, 1);

	buttons.add(valuesCC.addBoolParameter("Button 2", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button 1", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button B", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button A", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button -", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button C", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button Z", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button Home", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button Left", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button Right", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button Down", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button Up", "Button 2", false));
	buttons.add(valuesCC.addBoolParameter("Button Plus", "Button 2", false));

	
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
	activityTrigger->trigger();
}

void WiimoteModule::wiimoteButtonReleased(Wiimote * w, Wiimote::WiimoteButton b)
{
	if (w != device) return;
	buttons[(int)b]->setValue(false);
	activityTrigger->trigger();
}

void WiimoteModule::wiimoteOrientationUpdated(Wiimote * w)
{
	if (w != device) return;
	float targetPitch = -device->gforceY;
	pitch->setValue(pitch->floatValue()+(targetPitch - pitch->floatValue())*.5f);

	float r = -acosf(device->gforceX);
	if (device->gforceZ < 0) r = -r;
	r += float_Pi / 2;
	r /= float_Pi;
	if (r > 1) r -= 2;
	
	if (!isnan(r))
	{
		if (abs(r - roll->floatValue() > .5f)) roll->setValue(r);
		else roll->setValue(roll->floatValue() + (r - roll->floatValue())*.5f);
	}
}
