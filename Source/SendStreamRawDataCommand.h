/*
  ==============================================================================

    SendTCPRawDataCommand.h
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "BaseCommand.h"
#include "StreamingModule.h"

class SendStreamRawDataCommand :
	public BaseCommand
{
public:
	SendStreamRawDataCommand(StreamingModule * _module, CommandContext context, var params);
	~SendStreamRawDataCommand();

	StreamingModule * streamingModule;

	IntParameter * numBytes;
	ControllableContainer dataContainer;
	Array<IntParameter *> bytes;

	void updateBytesParams();
	void onContainerParameterChangedAsync(Parameter * p, const var &param) override;
	void trigger() override;

	static SendStreamRawDataCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendStreamRawDataCommand((StreamingModule *)module, context, params); }
};