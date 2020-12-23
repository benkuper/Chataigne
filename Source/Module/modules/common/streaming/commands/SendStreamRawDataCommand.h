/*
  ==============================================================================

    SendTCPRawDataCommand.h
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "SendStreamValuesCommand.h"

class SendStreamRawDataCommand :
	public SendStreamValuesCommand
{
public:
	SendStreamRawDataCommand(StreamingModule * _module, CommandContext context, var params, IteratorProcessor* iterator = nullptr);
	~SendStreamRawDataCommand();

	void triggerInternal() override;

	void customValueCreated(Parameter * p, var data);

	static SendStreamRawDataCommand * create(ControllableContainer * module, CommandContext context, var params, IteratorProcessor * iterator) { return new SendStreamRawDataCommand((StreamingModule *)module, context, params, iterator); }
};
