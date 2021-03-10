/*
  ==============================================================================

    KeyboardModuleCommands.h
    Created: 12 Mar 2020 4:03:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class KeyboardModuleCommands :
	public BaseCommand
{
public:
	KeyboardModuleCommands(KeyboardModule* m, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~KeyboardModuleCommands();

	KeyboardModule* keyboardModule;

	enum Type { KEY_DOWN, KEY_UP, KEY_HIT };
	Type type;

	EnumParameter* keyID;
	BoolParameter* ctrlPressed;
	BoolParameter* altPressed;
	BoolParameter* shiftPressed;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex * multiplex) { return new KeyboardModuleCommands((KeyboardModule*)module, context, params, multiplex); }

};