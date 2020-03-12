/*
  ==============================================================================

    KeyboardModule.cpp
    Created: 15 Mar 2018 9:36:44am
    Author:  Ben

  ==============================================================================
*/

#include "KeyboardModule.h"
#include "commands/KeyboardModuleCommands.h"

#if JUCE_WINDOWS
#include <windows.h>
#define KEYDOWN WM_KEYDOWN
#define KEYUP KEYEVENTF_KEYUP  
#define MIDDLE_DOWN MOUSEEVENTF_MIDDLEDOWN
#define MIDDLE_UP MOUSEEVENTF_MIDDLEUP
#define RIGHT_DOWN MOUSEEVENTF_RIGHTDOWN
#define RIGHT_UP MOUSEEVENTF_RIGHTUP
#else
#define LEFT_DOWN 0
#define LEFT_UP 1
#define MIDDLE_DOWN 2
#define MIDDLE_UP 3
#define RIGHT_DOWN 4
#define RIGHT_UP 5
#endif

KeyboardModule::KeyboardModule() :
	Module(getDefaultTypeString()),
	window(nullptr)
{
	setupIOConfiguration(true, true);

	window = TopLevelWindow::getActiveTopLevelWindow();
	if(window != nullptr) window->addKeyListener(this);
	
	lastKey = valuesCC.addStringParameter("Last Key", "Last Key pressed", "");
	ctrl = valuesCC.addBoolParameter("Ctrl", "Is Control down ?", false);
	shift = valuesCC.addBoolParameter("Shift", "Is shift down ?", false);
	command = valuesCC.addBoolParameter("Command", "Is command down ?", false);
	alt = valuesCC.addBoolParameter("Alt", "Is alt down ?", false);

	defManager->add(CommandDefinition::createDef(this, "", "Key Down", &KeyboardModuleCommands::create, CommandContext::ACTION)->addParam("type", KeyboardModuleCommands::KEY_DOWN));
	defManager->add(CommandDefinition::createDef(this, "", "Key Up", &KeyboardModuleCommands::create, CommandContext::ACTION)->addParam("type", KeyboardModuleCommands::KEY_UP));
	defManager->add(CommandDefinition::createDef(this, "", "Key hit", &KeyboardModuleCommands::create, CommandContext::ACTION)->addParam("type", KeyboardModuleCommands::KEY_HIT));

}

KeyboardModule::~KeyboardModule()
{
	if(TopLevelWindow::getActiveTopLevelWindow() == window) window->removeKeyListener(this);
}

void KeyboardModule::sendKeyDown(int keyID)
{
	if (!enabled->boolValue()) return;
	outActivityTrigger->trigger();

	MessageManagerLock mmLock;

#if JUCE_WINDOWS
	// Set up a generic keyboard event.
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.wVk = keyID; // virtual-key code for the "a" key
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));
#endif

}

void KeyboardModule::sendKeyUp(int keyID)
{
	if (!enabled->boolValue()) return;
	outActivityTrigger->trigger();

	MessageManagerLock mmLock;

#if JUCE_WINDOWS
	// Set up a generic keyboard event.
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.wVk = keyID; // virtual-key code for the "a" key
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));
#endif

}

void KeyboardModule::sendKeyHit(int keyID, bool ctrlPressed, bool altPressed, bool shiftPressed)
{
	if (ctrlPressed) sendKeyDown(VK_CONTROL);
	if (altPressed) sendKeyDown(VK_MENU);
	if (shiftPressed) sendKeyDown(VK_SHIFT);
	sendKeyDown(keyID);
	sendKeyUp(keyID);
	if (ctrlPressed) sendKeyUp(VK_CONTROL);
	if (altPressed) sendKeyUp(VK_MENU);
	if (shiftPressed) sendKeyUp(VK_SHIFT);

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
