/*
  ==============================================================================

    CommandFactory.cpp
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommandFactory.h"
#include "ModuleManager.h"

CommandDefinition::CommandDefinition(ControllableContainer * _container, const String & menuPath, const String & type, 
	CreateCommandFunc createFunc) :
	menuPath(menuPath),
	commandType(type),
	createFunc(createFunc),
	container(_container)
{
	params = var(new DynamicObject());
}

BaseCommand * CommandDefinition::create(CommandContext context)
{
	BaseCommand * c = createFunc(container, context, params);
	return c;
}

CommandDefinition * CommandDefinition::createDef(ControllableContainer * container, const String & menuPath, const String & type, CreateCommandFunc createFunc)
{
	return new CommandDefinition(container, menuPath, type, createFunc);
}

CommandDefinition * CommandDefinition::addParam(const String & paramName, var value)
{
	params.getDynamicObject()->setProperty(paramName, value);
	return this;
}

CommandDefinition * CommandFactory::showMenuAndGetCommand()
{
	PopupMenu m = ModuleManager::getInstance()->getAllModulesCommandMenu();
	int result = m.show();
	if (result > 0) return ModuleManager::getInstance()->getCommandDefinitionForItemID(result);

	return nullptr;
}
