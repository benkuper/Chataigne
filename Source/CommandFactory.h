/*
  ==============================================================================

    CommandFactory.h
    Created: 3 Nov 2016 12:24:41pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef COMMANDFACTORY_H_INCLUDED
#define COMMANDFACTORY_H_INCLUDED


#include "JuceHeader.h"
#include "BaseCommand.h"

typedef std::function<BaseCommand*(ControllableContainer *, CommandContext, var params)> CreateCommandFunc;

class CommandDefinition
{
public:
	String menuPath;
	String commandType;
	ControllableContainer * container;
	var params;

	CreateCommandFunc createFunc;

	CommandDefinition(ControllableContainer * container,
					  const String &menuPath, 
					  const String &type, 
					  CreateCommandFunc createFunc 
		);

	BaseCommand * create(CommandContext context);

	static CommandDefinition * createDef(ControllableContainer * container, const String &menuPath, const String &type, CreateCommandFunc createFunc);
	CommandDefinition * addParam(const String &paramName, var value);

};

class CommandFactory
{
public:
	static CommandDefinition * showMenuAndGetCommand();
};


#endif  // COMMANDFACTORY_H_INCLUDED
