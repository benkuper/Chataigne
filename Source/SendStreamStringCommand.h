/*
  ==============================================================================

    SendTCPStringCommand.h
    Created: 21 Oct 2017 5:05:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "BaseCommand.h"
#include "StreamingModule.h"

class SendStreamStringCommand :
	public BaseCommand
{
public:
	SendStreamStringCommand(StreamingModule * _module, CommandContext context, var params);
	~SendStreamStringCommand();

	StreamingModule * streamingModule;

	BoolParameter * appendCR;
	BoolParameter * appendNL;

	StringParameter * valueParam;

	void trigger() override;

	static SendStreamStringCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendStreamStringCommand((StreamingModule *)module, context, params); }

};