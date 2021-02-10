/*
  ==============================================================================

	ChataigneLogCommands.cpp
	Created: 13 Apr 2018 10:28:03am
	Author:  Ben

  ==============================================================================
*/

#include "ChataigneLogCommand.h"

ChataigneLogCommand::ChataigneLogCommand(ChataigneGenericModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	value(nullptr)
{
	type = (Type)(int)(params.getProperty("type", MESSAGE));

	logType = addEnumParameter("Log Type", "The Type of log");
	logType->addOption("Info", INFO)->addOption("Warning", WARNING)->addOption("Error", ERROR);

	message = addStringParameter("Message", "The message to log", type == VALUE ? "My value is" : "Wubba Lubba Dub Dub");

	if (type == VALUE)
	{
		if (context == ACTION) value = addTargetParameter("Target", "The target to log the value from");
		else if (context == MAPPING)
		{
			value = addStringParameter("Value", "The value that will be logged", "[notset]");
			value->setControllableFeedbackOnly(true);
			linkParamToMappingIndex(value, 0);
		}
	}
}

ChataigneLogCommand::~ChataigneLogCommand()
{
}

void ChataigneLogCommand::triggerInternal(int multiplexIndex)
{
	LogType lt = logType->getValueDataAsEnum<LogType>();

	String msg = getLinkedValue(message, multiplexIndex);

	switch (type)
	{
	case MESSAGE:
		switch (lt)
		{
		case INFO: LOG(msg); break;
		case WARNING: LOGWARNING(msg); break;
		case ERROR: LOGERROR(msg); break;
		}

		break;

	case VALUE:
	{
		var val = "[not set]";
		if (context == ACTION)
		{
			if (Parameter* p = dynamic_cast<Parameter*>(getLinkedParam(value)->getLinkedTarget(multiplexIndex).get()))
			{
				val = p->getValue();
			}
		}
		else val = getLinkedValue(value, multiplexIndex);
		
		String vString = value->type == Controllable::FLOAT ? String((float)val, 3) : val.toString();
		
		switch (lt)
		{
		case INFO: LOG(msg + " " + vString); break;
		case WARNING: LOGWARNING(msg + " " + vString);; break;
		case ERROR: LOGERROR(msg + " " + vString); break;
		}
	}
	break;
	}
}

BaseCommand* ChataigneLogCommand::create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex)
{
	return new ChataigneLogCommand((ChataigneGenericModule*)module, context, params, multiplex);

}
