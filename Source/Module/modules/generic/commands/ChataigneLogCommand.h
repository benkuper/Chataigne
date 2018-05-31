/*
  ==============================================================================

    ChataigneLogCommands.h
    Created: 13 Apr 2018 10:28:03am
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "Common/Command/BaseCommand.h"
#include "../ChataigneGenericModule.h"

class ChataigneLogCommand :
	public BaseCommand
{
public:
	ChataigneLogCommand(ChataigneGenericModule * _module, CommandContext context, var params);
	~ChataigneLogCommand();

	enum Type { MESSAGE, VALUE };
	Type type;

	StringParameter * message;
	TargetParameter * value;

	var logValue;

	void setValue(var value) override;
	void trigger() override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params);
};