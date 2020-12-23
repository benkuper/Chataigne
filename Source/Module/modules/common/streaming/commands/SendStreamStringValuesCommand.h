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
	SendStreamStringValuesCommand(StreamingModule * output, CommandContext context, var params, IteratorProcessor* iterator = nullptr);
	~SendStreamStringValuesCommand();

	StringParameter* prefix;
	StringParameter * separator;
	StringParameter* suffix;

	BoolParameter * appendCR;
	BoolParameter * appendNL;

	void triggerInternal() override;

	static SendStreamStringValuesCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new SendStreamStringValuesCommand((StreamingModule *)module, context, params, iterator); }

};
