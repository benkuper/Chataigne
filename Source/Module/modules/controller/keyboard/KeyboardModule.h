/*
  ==============================================================================

    KeyboardModule.h
    Created: 15 Mar 2018 9:36:44am
    Author:  Ben

  ==============================================================================
*/

#pragma once

using namespace juce;

class KeyboardModule :
	public Module
#if JUCE_WINDOWS
	,public KeyboardHooker::Listener
#else
	,public KeyListener
#endif
{
public:
	KeyboardModule();
	~KeyboardModule();

	TopLevelWindow * window;

	StringParameter* lastKey;
	IntParameter* lastKeyCode;
	StringParameter * combination;
	BoolParameter * ctrl;
	BoolParameter * shift;
	BoolParameter * command;
	BoolParameter * alt;

	ControllableContainer keysCC;
	HashMap<int, BoolParameter*> keyMap;

	void sendKeyDown(int keyID);
	void sendKeyUp(int keyID);
	void sendKeyHit(int keyID, bool ctrlPressed, bool altPressed, bool shiftPressed);


#if JUCE_WINDOWS
	void keyChanged(int keyCode, bool pressed);

#else
	virtual bool keyPressed(const KeyPress& key, Component* originatingComponent) override;
	virtual bool keyStateChanged(bool isKeyDown, Component* originatingComponent) override;
#endif

	static KeyboardModule * create() { return new KeyboardModule(); }
	virtual String getDefaultTypeString() const override { return "Keyboard"; }


};
