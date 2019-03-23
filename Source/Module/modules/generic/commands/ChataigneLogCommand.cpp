/*
  ==============================================================================

    ChataigneLogCommands.cpp
    Created: 13 Apr 2018 10:28:03am
    Author:  Ben

  ==============================================================================
*/

#include "ChataigneLogCommand.h"

ChataigneLogCommand::ChataigneLogCommand(ChataigneGenericModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	value(nullptr)
{
	type = (Type)(int)(params.getProperty("type", MESSAGE));

	message = addStringParameter("Message", "The message to log", type == VALUE?"My value is":"Wubba Lubba Dub Dub");
	if (type == VALUE)
	{
		if (context == ACTION) value = addTargetParameter("Target", "The target to log the value from");
		else if (context == MAPPING)
		{
			value = addStringParameter("Value", "The value that will be logged", "[notset]");
			value->setControllableFeedbackOnly(true);
		}
	}
}

ChataigneLogCommand::~ChataigneLogCommand()
{
}

void ChataigneLogCommand::setValue(var _value)
{
	BaseCommand::setValue(_value);
	if (_value.isArray() && _value.size() > 0)
	{
		String s = _value[0].toString();
		for (int i = 1; i < _value.size(); i++) s += ", "+_value[i].toString();
		((StringParameter *)value)->setValue(s);
	}else
	{
		((StringParameter *)value)->setValue(_value.toString());
	}
	
}

void ChataigneLogCommand::triggerInternal()
{
	switch (type)
	{
	case MESSAGE:
		LOG(message->stringValue());
		break;

	case VALUE:
	{
		String vString = context == ACTION ? String(((TargetParameter *)value)->target != nullptr?((Parameter *)((TargetParameter *)value)->target.get())->stringValue():"[not set]"):((StringParameter *)value)->stringValue();
		LOG(message->stringValue() + " " + vString);
	}
		break;
	}
}

BaseCommand * ChataigneLogCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new ChataigneLogCommand((ChataigneGenericModule *)module, context, params);

}
