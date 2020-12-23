/*
  ==============================================================================

    SendTCPRawDataCommand.cpp
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamRawDataCommand.h"

SendStreamRawDataCommand::SendStreamRawDataCommand(StreamingModule* _module, CommandContext context, var params, IteratorProcessor* iterator) :
	SendStreamValuesCommand(_module, context, params, iterator)
{
	customValuesManager->allowedTypes.add(Controllable::INT);
	customValuesManager->createParamCallbackFunc = std::bind(&SendStreamRawDataCommand::customValueCreated, this, std::placeholders::_1, std::placeholders::_2);
}

SendStreamRawDataCommand::~SendStreamRawDataCommand()
{
}

void SendStreamRawDataCommand::triggerInternal()
{
	StreamingCommand::triggerInternal(); //bypass StreamValueCommands to implement our own way

	Array<uint8> data;
	for (auto &i : customValuesManager->items) data.add(i->param->intValue());
	streamingModule->sendBytes(data);
}

void SendStreamRawDataCommand::customValueCreated(Parameter * p, var data)
{
	if (IntParameter* ip = dynamic_cast<IntParameter*>(p))
	{
		ip->hexMode = true;
		ip->setRange(0, 255);
	}
}
