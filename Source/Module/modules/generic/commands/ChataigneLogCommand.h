/*
  ==============================================================================

    ChataigneLogCommands.h
    Created: 13 Apr 2018 10:28:03am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ChataigneLogCommand :
	public BaseCommand
{
public:
	ChataigneLogCommand(ChataigneGenericModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~ChataigneLogCommand();

	enum Type { MESSAGE, VALUE, CLEAR };
	Type type;

	enum LogType { INFO, WARNING, ERROR };

	StringParameter * message;
	EnumParameter* logType;
	Parameter * value;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex = nullptr);
};
