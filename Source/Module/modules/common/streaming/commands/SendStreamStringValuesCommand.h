/*
  ==============================================================================

    SendStreamStringValuesCommand.h
    Created: 30 May 2018 12:01:59pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "SendStreamValuesCommand.h"

class SendStreamStringValuesCommand :
	public SendStreamValuesCommand
{
public:
	SendStreamStringValuesCommand(StreamingModule * output, CommandContext context, var params);
	~SendStreamStringValuesCommand();

	StringParameter * separator;
	BoolParameter * appendCR;
	BoolParameter * appendNL;

	void trigger() override;

	static SendStreamStringValuesCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendStreamStringValuesCommand((StreamingModule *)module, context, params); }

};
