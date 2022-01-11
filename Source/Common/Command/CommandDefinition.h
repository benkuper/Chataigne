/*
  ==============================================================================

    CommandDefinition.h
    Created: 19 Dec 2016 3:53:43pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class Multiplex;
class BaseCommand;

typedef std::function<BaseCommand*(ControllableContainer *, CommandContext, var params, Multiplex * multiplex)> CreateCommandFunc;

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

	BaseCommand * create(CommandContext context, Multiplex * multiplex = nullptr);

	static CommandDefinition * createDef(ControllableContainer * container, const String &menuPath, const String &type, CreateCommandFunc createFunc, CommandContext context = CommandContext::BOTH);
	CommandDefinition * addParam(const String &paramName, var value);

	WeakReference<CommandDefinition>::Master masterReference;
};