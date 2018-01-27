/*
  ==============================================================================

    SendTCPStringCommand.h
    Created: 21 Oct 2017 5:05:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "StreamingCommand.h"

class SendStreamStringCommand :
	public StreamingCommand
{
public:
	SendStreamStringCommand(StreamingModule * _module, CommandContext context, var params);
	~SendStreamStringCommand();


	BoolParameter * appendCR;
	BoolParameter * appendNL;

	StringParameter * prefix;
	StringParameter * valueParam;

	void trigger() override;

	static SendStreamStringCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendStreamStringCommand((StreamingModule *)module, context, params); }

};