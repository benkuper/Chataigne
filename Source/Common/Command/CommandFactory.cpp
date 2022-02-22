/*
  ==============================================================================

    CommandFactory.cpp
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

void CommandFactory::showMenuAndGetCommand(CommandContext context, std::function<void(CommandDefinition*)> returnFunc, Module* lockedModule, bool multiplexMode)
{
	PopupMenu m = getCommandMenu(context, lockedModule, multiplexMode);
	m.showMenuAsync(PopupMenu::Options(), [lockedModule, returnFunc](int result)
		{
			CommandDefinition* def = getCommandFromResult(result, lockedModule);
			if (def != nullptr) returnFunc(def);
		}
	);
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
