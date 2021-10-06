/*
  ==============================================================================

    CommandFactory.cpp
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommandFactory.h"
#include "Module/ModuleIncludes.h"

CommandDefinition* CommandFactory::showMenuAndGetCommand(CommandContext context, Module* lockedModule, bool multiplexMode)
{
	PopupMenu m = getCommandMenu(context, lockedModule, multiplexMode);
	return getCommandFromResult(m.show(), lockedModule);
}

PopupMenu CommandFactory::getCommandMenu(CommandContext context, Module* lockedModule, bool multiplexMode)
{
	return (lockedModule == nullptr) ? ModuleManager::getInstance()->getAllModulesCommandMenu(context, multiplexMode) : lockedModule->getCommandMenu(0, context);
}

CommandDefinition* CommandFactory::getCommandFromResult(int result, Module * lockedModule)
{
	if (result != 0) return ModuleManager::getInstance()->getCommandDefinitionForItemID(result, lockedModule);

	return nullptr;
}
