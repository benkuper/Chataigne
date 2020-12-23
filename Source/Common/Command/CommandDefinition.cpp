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

CommandDefinition::~CommandDefinition() 
{
	masterReference.clear();
}

void CommandDefinition::setup(ControllableContainer * _container, const String & _menuPath, const String & _commandType, CommandContext _context,
	CreateCommandFunc _createFunc)
{
	this->menuPath = _menuPath;
	this->commandType = _commandType;
	this->createFunc = _createFunc;
	this->container = _container;
	this->context = _context;

	params = var(new DynamicObject());
}


BaseCommand * CommandDefinition::create(CommandContext _context, IteratorProcessor * iterator)
{
	BaseCommand * c = createFunc(container, _context, params, iterator);
	return c;
}

CommandDefinition * CommandDefinition::createDef(ControllableContainer * container, const String & menuPath, const String & type, CreateCommandFunc createFunc, CommandContext context)
{
	CommandDefinition *def = new CommandDefinition();
	def->setup(container, menuPath, type, context, createFunc);
	return def;
}

CommandDefinition * CommandDefinition::addParam(const String & paramName, var value)
{
	params.getDynamicObject()->setProperty(paramName, value);
	return this;
}