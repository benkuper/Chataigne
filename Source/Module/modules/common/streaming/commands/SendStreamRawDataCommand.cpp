/*
  ==============================================================================

    SendTCPRawDataCommand.cpp
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamRawDataCommand.h"

SendStreamRawDataCommand::SendStreamRawDataCommand(StreamingModule * _module, CommandContext context, var params) :
	SendStreamValuesCommand(_module,context, params)
{
	customValuesManager->allowedTypes.add(Controllable::INT);
}

SendStreamRawDataCommand::~SendStreamRawDataCommand()
{
}

void SendStreamRawDataCommand::triggerInternal()
{
	StreamingCommand::triggerInternal();

	Array<uint8> data;
	for (auto &i : customValuesManager->items) data.add(i->param->intValue());
	streamingModule->sendBytes(data);
}
