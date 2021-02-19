/*
  ==============================================================================

    CommandFactory.cpp
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommandFactory.h"
#include "Module/ModuleManager.h"

CommandDefinition* CommandFactory::showMenuAndGetCommand(CommandContext context, Module* lockedModule, bool multiplexMode)
{
	PopupMenu m = (lockedModule == nullptr) ? ModuleManager::getInstance()->getAllModulesCommandMenu(context, multiplexMode) : lockedModule->getCommandMenu(0, context);
	
	int result = m.show();
	if (result != 0) return ModuleManager::getInstance()->getCommandDefinitionForItemID(result, lockedModule);

	return nullptr;
}