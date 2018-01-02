/*
  ==============================================================================

    SendTCPStringCommand.h
    Created: 21 Oct 2017 5:05:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "BaseCommand.h"
#include "TCPModule.h"

class SendTCPStringCommand :
	public BaseCommand
{
public:
	SendTCPStringCommand(TCPModule * _module, CommandContext context, var params);
	~SendTCPStringCommand();

	TCPModule * tcpModule;

	BoolParameter * appendCR;
	BoolParameter * appendNL;

	StringParameter * valueParam;

	void trigger() override;

	static SendTCPStringCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendTCPStringCommand((TCPModule *)module, context, params); }

};