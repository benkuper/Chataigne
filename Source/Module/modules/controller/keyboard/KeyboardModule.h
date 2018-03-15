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

	static KeyboardModule * create() { return new KeyboardModule(); }
	virtual String getDefaultTypeString() const override { return "Keyboard"; }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyboardModule)

		// Inherited via KeyListener
		virtual bool keyPressed(const KeyPress & key, Component * originatingComponent) override;
};