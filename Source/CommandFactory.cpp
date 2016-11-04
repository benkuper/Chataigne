/*
  ==============================================================================

    CommandFactory.cpp
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommandFactory.h"
#include "OutputManager.h"

CommandDefinition::CommandDefinition(ControllableContainer * _container, const String & menuPath, const String & type, 
	CreateCommandFunc createFunc) :
	menuPath(menuPath),
	inputType(type),
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
	PopupMenu m = OutputManager::getInstance()->getAllOutputsCommandMenu();
	int result = m.show();
	if (result > 0) return OutputManager::getInstance()->getCommandDefinitionForItemID(result);

	return nullptr;
}
