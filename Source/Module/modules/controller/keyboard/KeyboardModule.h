/*
  ==============================================================================

    KeyboardModule.h
    Created: 15 Mar 2018 9:36:44am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"

class KeyboardModule :
	public Module,
	public KeyListener
{
public:
	KeyboardModule();
	~KeyboardModule();

	TopLevelWindow * window;

	StringParameter * lastKey;
	BoolParameter * ctrl;
	BoolParameter * shift;
	BoolParameter * command;
	BoolParameter * alt;

	void sendKeyDown(int keyID);
	void sendKeyUp(int keyID);
	void sendKeyHit(int keyID, bool ctrlPressed, bool altPressed, bool shiftPressed);

	virtual bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
	virtual bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;

	static KeyboardModule * create() { return new KeyboardModule(); }
	virtual String getDefaultTypeString() const override { return "Keyboard"; }

};