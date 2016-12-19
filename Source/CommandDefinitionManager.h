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
#include "BaseManager.h"

class CommandDefinitionManager :
	public BaseManager<CommandDefinition>
{
public:
	CommandDefinitionManager();
	~CommandDefinitionManager();

	PopupMenu commandMenu;
	void rebuildCommandMenu(int baseID);
	PopupMenu getCommandMenu(int baseID);
	CommandDefinition * getCommandDefinitionFor(const String &menuPath, const String &moduleType);
};



#endif  // COMMANDDEFINITIONMANAGER_H_INCLUDED
