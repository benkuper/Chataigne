/*
  ==============================================================================

    SendTCPStringCommand.cpp
    Created: 21 Oct 2017 5:05:39pm
    Author:  Ben

  ==============================================================================
*/

#include "SendTCPStringCommand.h"

SendTCPStringCommand::SendTCPStringCommand(TCPModule * _module, CommandContext context, var params) :
	BaseCommand(_module,context,params),
	tcpModule(_module)
{
	valueParam = addStringParameter("Value", "Value to send via TCP", "example");
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

SendTCPStringCommand::~SendTCPStringCommand()
{
}

void SendTCPStringCommand::trigger()
{
	tcpModule->sendStringPacket(valueParam->stringValue()+(appendCR->boolValue()?"\r":"")+(appendNL->boolValue()?"\n":""));
}
