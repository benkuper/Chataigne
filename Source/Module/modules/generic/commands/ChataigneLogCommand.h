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
	ChataigneLogCommand(ChataigneGenericModule * _module, CommandContext context, var params, IteratorProcessor* iterator = nullptr);
	~ChataigneLogCommand();

	enum Type { MESSAGE, VALUE };
	Type type;

	enum LogType { INFO, WARNING, ERROR };

	StringParameter * message;
	EnumParameter* logType;
	Parameter * value;

	void setValueInternal(var value, int iterationIndex) override;
	void triggerInternal(int iterationIndex) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator = nullptr);
};
