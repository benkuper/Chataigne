/*
  ==============================================================================

    CommandFactory.cpp
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommandFactory.h"
#include "ModuleManager.h"


CommandDefinition * CommandFactory::showMenuAndGetCommand()
{
	PopupMenu m = ModuleManager::getInstance()->getAllModulesCommandMenu();
	int result = m.show();
	if (result > 0) return ModuleManager::getInstance()->getCommandDefinitionForItemID(result);

	return nullptr;
}
