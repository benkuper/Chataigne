/*
  ==============================================================================

    JoystickModule.cpp
    Created: 17 Jun 2018 9:32:37pm
    Author:  Ben

  ==============================================================================
*/

JoystickModule::JoystickModule(const String & name) :
	Module(name),
	calibCC("Calibration")
{
	setupIOConfiguration(true, false);
	joystickParam = new JoystickParameter("Device", "The Joystick to connect to");
	moduleParams.addParameter(joystickParam);
	for (int i = 0; i < 4; ++i)
	{
		axisOffset.add(calibCC.addFloatParameter("Axis "+String(i+1)+" Offset", "Offset if axis is not centered", 0, -1, 1));
		axisDeadzone.add(calibCC.addFloatParameter("Axis " + String(i + 1) + " Dead zone", "Percentage of dead zone in the center to avoid noisy input", 0, 0, 1));
	}

	moduleParams.addChildControllableContainer(&calibCC);

	InputSystemManager::getInstance()->addInputManagerListener(this);
}

JoystickModule::~JoystickModule()
{
	joystickParam->setJoystick(nullptr);
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
	else if (cc == &valuesCC)
	{
		inActivityTrigger->trigger();
	}
	
	if (c == joystickParam || c->parentContainer == &calibCC)
	{
		if (joystickParam->joystick != nullptr)
		{
			for (int i = 0; i < 4; ++i)
			{
				joystickParam->joystick->axisOffset[i] = axisOffset[i]->floatValue();
				joystickParam->joystick->axisDeadZone[i] = axisDeadzone[i]->floatValue();
			}
			joystickParam->joystick->update();
			
		}
	}
}
