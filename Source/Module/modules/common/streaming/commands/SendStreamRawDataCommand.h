/*
  ==============================================================================

    SendTCPRawDataCommand.h
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "StreamingCommand.h"

class SendStreamRawDataCommand :
	public StreamingCommand
{
public:
	SendStreamRawDataCommand(StreamingModule * _module, CommandContext context, var params);
	~SendStreamRawDataCommand();

	IntParameter * numBytes;
	ControllableContainer dataContainer;
	IntParameter * targetMappingByteIndex;
	Array<IntParameter *> bytes;

	void updateBytesParams();
	void onContainerParameterChangedAsync(Parameter * p, const var &param) override;
	void trigger() override;

	static SendStreamRawDataCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendStreamRawDataCommand((StreamingModule *)module, context, params); }
};