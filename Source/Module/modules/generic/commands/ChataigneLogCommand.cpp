/*
  ==============================================================================

	ChataigneLogCommands.cpp
	Created: 13 Apr 2018 10:28:03am
	Author:  Ben

  ==============================================================================
*/

#include "ChataigneLogCommand.h"

ChataigneLogCommand::ChataigneLogCommand(ChataigneGenericModule* _module, CommandContext context, var params, IteratorProcessor* iterator) :
	BaseCommand(_module, context, params, iterator),
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

void ChataigneLogCommand::setValueInternal(var _value, int iterationIndex)
{
	if (_value.isArray() && _value.size() > 0)
	{
		String s = _value[0].toString();
		for (int i = 1; i < _value.size(); ++i) s += ", " + _value[i].toString();
		((StringParameter*)value)->setValue(s);
	}
	else
	{
		((StringParameter*)value)->setValue(_value.isDouble() ? String((float)_value,3):_value.toString());
	}
}

void ChataigneLogCommand::triggerInternal(int iterationIndex)
{
	LogType lt = logType->getValueDataAsEnum<LogType>();

	switch (type)
	{
	case MESSAGE:

		switch (lt)
		{
		case INFO: LOG(message->stringValue()); break;
		case WARNING: LOGWARNING(message->stringValue()); break;
		case ERROR: LOGERROR(message->stringValue()); break;

		}

		break;

	case VALUE:
	{
		String vString = "[not set]";
		Parameter* p = nullptr;
		if (context == ACTION)  p = dynamic_cast<Parameter*>(((TargetParameter*)value)->target.get());
		else p = value;
		
		if (p != nullptr) vString = p->type == Controllable::FLOAT ? String(p->floatValue(), 3) : p->stringValue();
	
		switch (lt)
		{
		case INFO: LOG(message->stringValue() + " " + vString); break;
		case WARNING: LOGWARNING(message->stringValue() + " " + vString);; break;
		case ERROR: LOGERROR(message->stringValue() + " " + vString); break;

		}
	}
	break;
	}
}

BaseCommand* ChataigneLogCommand::create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator)
{
	return new ChataigneLogCommand((ChataigneGenericModule*)module, context, params, iterator);

}
