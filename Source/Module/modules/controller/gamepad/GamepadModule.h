/*
  ==============================================================================

    GamepadModule.h
    Created: 26 Dec 2016 4:56:31pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class GamepadModule :
	public Module,
	public InputSystemManager::InputManagerListener,
	public Gamepad::GamepadListener
{
public:
	GamepadModule(const String &name = "Gamepad");
	~GamepadModule();

	GamepadParameter * gamepadParam;
	Gamepad* gamepad;
	WeakReference<Gamepad> gamepadRef;

	ControllableContainer axesCC;
	ControllableContainer buttonsCC;

	ControllableContainer calibCC;
	Array<FloatParameter*> axisOffset;
	Array<FloatParameter*> axisDeadzone;
	
	//void rebuildValues();

	void setGamepad(Gamepad* g);

	void gamepadAdded(Gamepad *) override;
	void gamepadRemoved(Gamepad *) override;

	void gamepadValuesUpdated(Array<float> axes, Array<bool> buttons) override;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	static GamepadModule * create() { return new GamepadModule(); }
	virtual String getDefaultTypeString() const override { return "Gamepad"; }

};

