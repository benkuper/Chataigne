/*
  ==============================================================================

    CommandDefinition.cpp
    Created: 19 Dec 2016 3:53:43pm
    Author:  Ben

  ==============================================================================
*/

#include "CommandDefinition.h"

CommandDefinition::CommandDefinition()
{
}

void CommandDefinition::setup(ControllableContainer * _container, const String & _menuPath, const String & _commandType,
	CreateCommandFunc _createFunc)
{
	this->menuPath = _menuPath;
	this->commandType = _commandType;
	this->createFunc = _createFunc;
	this->container = _container;
	//setNiceName(menuPath.isNotEmpty()?menuPath+"_"+commandType:commandType);

	params = var(new DynamicObject());
}


BaseCommand * CommandDefinition::create(CommandContext context)
{
	BaseCommand * c = createFunc(container, context, params);
	return c;
}

CommandDefinition * CommandDefinition::createDef(ControllableContainer * container, const String & menuPath, const String & type, CreateCommandFunc createFunc)
{
	CommandDefinition *def = new CommandDefinition();
	def->setup(container, menuPath, type, createFunc);
	return def;
}

CommandDefinition * CommandDefinition::addParam(const String & paramName, var value)
{
	params.getDynamicObject()->setProperty(paramName, value);
	return this;
}