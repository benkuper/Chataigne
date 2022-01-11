/*
  ==============================================================================

    CommandDefinitionManager.h
    Created: 19 Dec 2016 3:48:25pm
    Author:  Ben

  ==============================================================================
*/


#pragma once

class CommandDefinition;

class CommandDefinitionManager
{
public:
	CommandDefinitionManager();
	~CommandDefinitionManager();

	OwnedArray<CommandDefinition> definitions;

	void add(CommandDefinition* def, int index = -1);
	void remove(CommandDefinition * def);
	void clear();

	PopupMenu getCommandMenu(int baseID, CommandContext context);
	void addCommandsToMenu(PopupMenu * m, int baseID, CommandContext context);

	CommandDefinition * getCommandDefinitionFor(const String &menuPath, const String &moduleType);
};