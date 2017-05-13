/*
  ==============================================================================

    CommandDefinitionManager.h
    Created: 19 Dec 2016 3:48:25pm
    Author:  Ben

  ==============================================================================
*/

#ifndef COMMANDDEFINITIONMANAGER_H_INCLUDED
#define COMMANDDEFINITIONMANAGER_H_INCLUDED

#include "CommandDefinition.h"

class CommandDefinitionManager
{
public:
	CommandDefinitionManager();
	~CommandDefinitionManager();

	OwnedArray<CommandDefinition> definitions;

	void add(CommandDefinition * def);
	void remove(CommandDefinition * def);

	PopupMenu commandMenu;
	void rebuildCommandMenu(int baseID, CommandContext context);
	PopupMenu getCommandMenu(int baseID, CommandContext context);
	CommandDefinition * getCommandDefinitionFor(const String &menuPath, const String &moduleType);
};



#endif  // COMMANDDEFINITIONMANAGER_H_INCLUDED
