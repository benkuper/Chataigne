/*
  ==============================================================================

    SendStreamStringCommand.cpp
    Created: 21 Oct 2017 5:05:39pm
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamStringCommand.h"

SendStreamStringCommand::SendStreamStringCommand(StreamingModule * _module, CommandContext context, var params) :
	StreamingCommand(_module,context,params),
	prefix(nullptr)
{
	if (context == CommandContext::MAPPING)
	{
		prefix = addStringParameter("Prefix", "Add some data before the mapped value\nThis does NOT automatically append a whitespace !","");
	}

	valueParam = addStringParameter("Value", "Value to send via TCP", "example");

	if (context == CommandContext::MAPPING)
	{
		setTargetMappingParameterAt(valueParam, 0);
	}

	if (params.hasProperty("fixedValue"))
	{
		valueParam->isEditable = false;
		valueParam->setValue(params.getProperty("fixedValue", ""));
	}
	
	appendCR = addBoolParameter("Append CR", "Append \\r at the end of the message", true);
	if (params.hasProperty("forceCR"))
	{
		appendCR->setValue(params.getProperty("forceCR", true));
		appendCR->hideInEditor = true;
	}

	appendNL = addBoolParameter("Append NL", "Append \\n at the end of the message", true);
	if (params.hasProperty("forceNL"))
	{
		appendNL->setValue(params.getProperty("forceNL", true)); 
		appendNL->hideInEditor = true;
	}
}

SendStreamStringCommand::~SendStreamStringCommand()
{
}

void SendStreamStringCommand::trigger()
{
	String m = valueParam->stringValue() + (appendCR->boolValue() ? "\r" : "") + (appendNL->boolValue() ? "\n" : "");
	if (prefix != nullptr) m = prefix->stringValue() + m;
	streamingModule->sendMessage(m);
}
