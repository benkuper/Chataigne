/*
  ==============================================================================

    KeyboardModuleCommands.cpp
    Created: 12 Mar 2020 4:03:59pm
    Author:  bkupe

  ==============================================================================
*/

KeyboardModuleCommands::KeyboardModuleCommands(KeyboardModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	keyboardModule(_module),
	ctrlPressed(nullptr),
	altPressed(nullptr),
	shiftPressed(nullptr)
{
	type = (Type)(int)params.getProperty("type", KEY_HIT);
	
	keyID = addEnumParameter("Key", "The key to hit");

	HashMap<int, String>::Iterator it(WindowsHooker::getInstance()->keyMap);
	while(it.next())
	{
		keyID->addOption(it.getValue(), it.getKey());
	}

	if (type == KEY_HIT)
	{
		ctrlPressed = addBoolParameter("Ctrl Pressed", "Simulate ctrl (or command on Mac) pressed", false);
		altPressed = addBoolParameter("Alt Pressed", "Simulate alt pressed", false);
		shiftPressed = addBoolParameter("Shift pressed", "Simulate shift pressed", false);
	}
}

KeyboardModuleCommands::~KeyboardModuleCommands()
{
}

void KeyboardModuleCommands::triggerInternal(int multiplexIndex)
{
	switch (type)
	{
	case KEY_DOWN: keyboardModule->sendKeyDown(keyID->getValueData()); break;
	case KEY_UP: keyboardModule->sendKeyUp(keyID->getValueData()); break;
	case KEY_HIT: keyboardModule->sendKeyHit(keyID->getValueData(), getLinkedValue(ctrlPressed, multiplexIndex), getLinkedValue(altPressed, multiplexIndex), getLinkedValue(shiftPressed, multiplexIndex)); break;
	}
}
