/*
  ==============================================================================

    GamepadModule.h
    Created: 26 Dec 2016 4:56:31pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Common/InputSystem/InputSystemManager.h"

class GamepadModule :
	public Module,
	public InputSystemManager::InputManagerListener
{
public:
	GamepadModule(const String &name = "Gamepad");
	~GamepadModule();

	GamepadParameter * gamepadParam;

	ControllableContainer calibCC;
	Array<FloatParameter*> axisOffset;
	Array<FloatParameter*> axisDeadzone;
	
	void rebuildValues();

	void gamepadAdded(Gamepad *) override;
	void gamepadRemoved(Gamepad *) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static GamepadModule * create() { return new GamepadModule(); }
	virtual String getDefaultTypeString() const override { return "Gamepad"; }

};

