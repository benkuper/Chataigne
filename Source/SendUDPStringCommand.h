/*
  ==============================================================================

    SendUDPStringCommand.h
    Created: 2 Jan 2018 12:55:00pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "BaseCommand.h"
#include "UDPModule.h"

class SendUDPStringCommand :
	public BaseCommand
{
public:
	SendUDPStringCommand(UDPModule * _module, CommandContext context, var params);
	~SendUDPStringCommand();

	UDPModule * tcpModule;
	StringParameter * valueParam;

	void trigger() override;

	static SendUDPStringCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendUDPStringCommand((UDPModule *)module, context, params); }

};