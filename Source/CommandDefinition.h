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
#include "BaseItem.h"

typedef std::function<BaseCommand*(ControllableContainer *, CommandContext, var params)> CreateCommandFunc;

class CommandDefinition :
	public BaseItem
{
public:
	String menuPath;
	String commandType;
	ControllableContainer * container;
	var params;

	CreateCommandFunc createFunc;

	CommandDefinition();

	void setup(ControllableContainer * container,
		const String &menuPath,
		const String &type,
		CreateCommandFunc createFunc);

	BaseCommand * create(CommandContext context);

	static CommandDefinition * createDef(ControllableContainer * container, const String &menuPath, const String &type, CreateCommandFunc createFunc);
	CommandDefinition * addParam(const String &paramName, var value);

};


#endif  // COMMANDDEFINITION_H_INCLUDED
