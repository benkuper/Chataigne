/*
  ==============================================================================

	InputDeviceHelpers.h
	Created: 17 Jun 2018 8:45:30pm
	Author:  Ben

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "InputSystemManager.h"

class GamepadParameterUI :
	public ParameterUI,
	public InputSystemManager::AsyncListener,
	public ComboBox::Listener
{
public:
	GamepadParameterUI(Array<GamepadParameter*> gamepads);
	~GamepadParameterUI();

	Array<GamepadParameter*> gamepadParams;
	GamepadParameter* gamepadParam;
	ComboBox chooser;

	Array<WeakReference<Gamepad>> gamepads;
	WeakReference<Gamepad> getGamepad();

	void rebuild();
	void resized() override;

	void valueChanged(const var&) override;

	void newMessage(const InputSystemManager::InputSystemEvent& e) override;
	virtual void comboBoxChanged(ComboBox*) override;
};
