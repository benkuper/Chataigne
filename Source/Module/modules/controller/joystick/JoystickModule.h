/*
  ==============================================================================

    JoystickModule.h
    Created: 17 Jun 2018 9:32:37pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "Common/InputSystem/InputSystemManager.h"

class JoystickModule :
	public Module,
	public InputSystemManager::InputManagerListener
{
public:
	JoystickModule(const String &name = "Joystick");
	~JoystickModule();

	JoystickParameter* joystickParam;

	ControllableContainer calibCC;
	Array<FloatParameter *> axisOffset;
	Array<FloatParameter *> axisDeadzone;

	void rebuildValues();

	void joystickAdded(Joystick *) override;
	void joystickRemoved(Joystick *) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static JoystickModule * create() { return new JoystickModule(); }
	virtual String getDefaultTypeString() const override { return "Joystick"; }



};
