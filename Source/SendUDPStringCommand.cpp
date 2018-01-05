/*
  ==============================================================================

    SendUDPStringCommand.cpp
    Created: 2 Jan 2018 12:55:00pm
    Author:  Ben

  ==============================================================================
*/

#include "SendUDPStringCommand.h"

SendUDPStringCommand::SendUDPStringCommand(UDPModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	udpModule(_module)
{
	valueParam = addStringParameter("Value", "Value to send via UDP", "example");
	if (params.hasProperty("fixedValue"))
	{
		valueParam->isEditable = false;
		valueParam->setValue(params.getProperty("fixedValue", ""));
	}
}

SendUDPStringCommand::~SendUDPStringCommand()
{
}

void SendUDPStringCommand::trigger()
{
	udpModule->sendMessage(valueParam->stringValue());
}
