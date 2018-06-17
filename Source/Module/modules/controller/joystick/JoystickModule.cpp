/*
  ==============================================================================

    JoystickModule.cpp
    Created: 17 Jun 2018 9:32:37pm
    Author:  Ben

  ==============================================================================
*/

#include "JoystickModule.h"

JoystickModule::JoystickModule(const String & name) :
	Module(name)
{
	setupIOConfiguration(true, false);
	joystickParam = new JoystickParameter("Device", "The Joystick to connect to");
	moduleParams.addParameter(joystickParam);
}

JoystickModule::~JoystickModule()
{
}

void JoystickModule::rebuildValues()
{
	while (valuesCC.controllableContainers.size() > 0) valuesCC.removeChildControllableContainer(valuesCC.controllableContainers[0]);

	valuesCC.clear();
	if (joystickParam == nullptr) return;
	if (joystickParam->joystick == nullptr) return;
	valuesCC.addChildControllableContainer(&joystickParam->joystick->axesCC);
	valuesCC.addChildControllableContainer(&joystickParam->joystick->buttonsCC);
}

void JoystickModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c == joystickParam)
	{
		rebuildValues();
	}
}
