/*
  ==============================================================================

    JoystickModule.cpp
    Created: 17 Jun 2018 9:32:37pm
    Author:  Ben

  ==============================================================================
*/

#include "JoystickModule.h"

#if JUCE_WINDOWS

JoystickModule::JoystickModule(const String & name) :
	Module(name)
{
	setupIOConfiguration(true, false);
	joystickParam = new JoystickParameter("Device", "The Joystick to connect to");
	moduleParams.addParameter(joystickParam);
	InputSystemManager::getInstance()->addInputManagerListener(this);
}

JoystickModule::~JoystickModule()
{
	if (InputSystemManager::getInstanceWithoutCreating() != nullptr) InputSystemManager::getInstance()->removeInputManagerListener(this);
}

void JoystickModule::rebuildValues()
{
	while (valuesCC.controllableContainers.size() > 0) valuesCC.removeChildControllableContainer(valuesCC.controllableContainers[0]);

	valuesCC.clear();
	if (joystickParam->joystick == nullptr) return;

	valuesCC.addChildControllableContainer(&joystickParam->joystick->axesCC);
	valuesCC.addChildControllableContainer(&joystickParam->joystick->buttonsCC);
}

void JoystickModule::joystickAdded(Joystick * j)
{
	String jName = String(SDL_JoystickName(j->joystick));
	if (jName == joystickParam->ghostName)
	{
		joystickParam->setJoystick(j);
		rebuildValues();
	}
}

void JoystickModule::joystickRemoved(Joystick * j)
{
	if (joystickParam->joystick == j)
	{
		valuesCC.removeChildControllableContainer(&joystickParam->joystick->axesCC);
		valuesCC.removeChildControllableContainer(&joystickParam->joystick->buttonsCC);
	}

}

void JoystickModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);
	if (c == joystickParam)
	{
		rebuildValues();
	}
}
#endif
