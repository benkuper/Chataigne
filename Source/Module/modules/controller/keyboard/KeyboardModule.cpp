/*
  ==============================================================================

    KeyboardModule.cpp
    Created: 15 Mar 2018 9:36:44am
    Author:  Ben

  ==============================================================================
*/

#include "KeyboardModule.h"

KeyboardModule::KeyboardModule() :
	Module(getDefaultTypeString()),
	window(nullptr)
{
	setupIOConfiguration(true, false);

	window = TopLevelWindow::getActiveTopLevelWindow();
	if(window != nullptr) window->addKeyListener(this);
	
	lastKey = valuesCC.addStringParameter("Last Key", "Last Key pressed", "");
	ctrl = valuesCC.addBoolParameter("Ctrl", "Is Control down ?", false);
	shift = valuesCC.addBoolParameter("Shift", "Is shift down ?", false);
	command = valuesCC.addBoolParameter("Command", "Is command down ?", false);
	alt = valuesCC.addBoolParameter("Alt", "Is alt down ?", false);
}

KeyboardModule::~KeyboardModule()
{
	if(TopLevelWindow::getActiveTopLevelWindow() == window) window->removeKeyListener(this);
}

bool KeyboardModule::keyPressed(const KeyPress & key, Component * originatingComponent)
{
	char k = (char)key.getKeyCode();
	String ks = String::fromUTF8(&k, 1);
	lastKey->setValue(ks.toLowerCase());

	shift->setValue(key.getModifiers().isShiftDown());
	ctrl->setValue(key.getModifiers().isCtrlDown());
	command->setValue(key.getModifiers().isCommandDown());
	alt->setValue(key.getModifiers().isAltDown());

	return false;
}

bool KeyboardModule::keyStateChanged(bool isKeyDown, Component * originatingComponent)
{
	if (!isKeyDown)
	{
		lastKey->setValue("");
		ctrl->setValue(false);
		shift->setValue(false);
		command->setValue(false);
		alt->setValue(false);
	}
	return false;
}
