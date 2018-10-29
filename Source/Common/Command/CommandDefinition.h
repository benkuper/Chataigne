/*
  ==============================================================================

    CommandDefinition.h
    Created: 19 Dec 2016 3:53:43pm
    Author:  Ben

  ==============================================================================
*/

#ifndef COMMANDDEFINITION_H_INCLUDED
#define COMMANDDEFINITION_H_INCLUDED

#include "BaseCommand.h"
#include "CommandContext.h"

typedef std::function<BaseCommand*(ControllableContainer *, CommandContext, var params)> CreateCommandFunc;

class CommandDefinition
{
public:
	CommandDefinition();
	virtual ~CommandDefinition();
	
	CommandContext context;
	String menuPath;
	String commandType;
	ControllableContainer * container;
	var params;

	CreateCommandFunc createFunc;

	void setup(ControllableContainer * container,
		const String &menuPath,
		const String &type,
		CommandContext context,
		CreateCommandFunc createFunc);

	BaseCommand * create(CommandContext context);

	static CommandDefinition * createDef(ControllableContainer * container, const String &menuPath, const String &type, CreateCommandFunc createFunc, CommandContext context = CommandContext::BOTH);
	CommandDefinition * addParam(const String &paramName, var value);

	WeakReference<CommandDefinition>::Master masterReference;
};


#endif  // COMMANDDEFINITION_H_INCLUDED
