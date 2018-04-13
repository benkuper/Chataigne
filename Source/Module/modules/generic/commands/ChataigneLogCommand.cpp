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

	message = addStringParameter("Message", "The message to log", "Wubba Lubba Dub Dub");
	if(type == VALUE) value = addTargetParameter("Target", "The target to log the value from");
}

ChataigneLogCommand::~ChataigneLogCommand()
{
}

void ChataigneLogCommand::trigger()
{
	switch (type)
	{
	case MESSAGE:
		LOG(message->stringValue());
		break;

	case VALUE:
		LOG(message->stringValue() + " " + String(value->target != nullptr ? ((Parameter *)value->target.get())->value.toString() : " not set"));
		break;
	}
}

BaseCommand * ChataigneLogCommand::create(ControllableContainer * module, CommandContext context, var params)
{
	return new ChataigneLogCommand((ChataigneGenericModule *)module, context, params);

}
