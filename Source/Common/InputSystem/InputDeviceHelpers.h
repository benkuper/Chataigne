/*
  ==============================================================================

    InputDeviceHelpers.h
    Created: 17 Jun 2018 8:45:30pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "InputSystemManager.h"

class JoystickParameterUI :
	public ParameterUI,
	public InputSystemManager::AsyncListener,
	public ComboBox::Listener
{
public :
	JoystickParameterUI(JoystickParameter * p);
	~JoystickParameterUI();

	JoystickParameter * joystickParam;
	ComboBox chooser;

	Array<WeakReference<Joystick>> joysticks;
	WeakReference<Joystick> getJoystick();

	void rebuild();
	void resized();

	void newMessage(const InputSystemManager::InputSystemEvent &e) override;

	virtual void comboBoxChanged(ComboBox *) override;
};

class GamepadParameterUI :
	public ParameterUI,
	public InputSystemManager::AsyncListener,
	public ComboBox::Listener
{
public:
	GamepadParameterUI(GamepadParameter * p);
	~GamepadParameterUI();

	GamepadParameter * gamepadParam;
	ComboBox chooser;

	Array<WeakReference<Gamepad>> gamepads;
	WeakReference<Gamepad> getGamepad();

	void rebuild();
	void resized();

	void valueChanged(const var &) override;

	void newMessage(const InputSystemManager::InputSystemEvent &e) override;
	virtual void comboBoxChanged(ComboBox *) override;
};
