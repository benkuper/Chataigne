/*
  ==============================================================================

    CommandFactory.cpp
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommandFactory.h"
#include "ModuleManager.h"


CommandDefinition * CommandFactory::showMenuAndGetCommand(CommandContext context, Module * lockedModule)
{
	PopupMenu m = (lockedModule == nullptr) ? ModuleManager::getInstance()->getAllModulesCommandMenu(context) : lockedModule->defManager.getCommandMenu(0, context);
	int result = m.show();
	if (result > 0) return ModuleManager::getInstance()->getCommandDefinitionForItemID(result,lockedModule);

	return nullptr;
}