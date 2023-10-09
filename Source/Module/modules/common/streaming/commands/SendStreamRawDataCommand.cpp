/*
  ==============================================================================

    SendTCPRawDataCommand.cpp
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

SendStreamRawDataCommand::SendStreamRawDataCommand(StreamingModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	SendStreamValuesCommand(_module, context, params, multiplex)
{
	customValuesManager->allowedTypes.add(Controllable::INT);
	customValuesManager->createParamCallbackFunc = std::bind(&SendStreamRawDataCommand::customValueCreated, this, std::placeholders::_1);
	customValuesManager->enablePrecison = false;
}

SendStreamRawDataCommand::~SendStreamRawDataCommand()
{
}

void SendStreamRawDataCommand::triggerInternal(int multiplexIndex)
{
	StreamingCommand::triggerInternal(multiplexIndex); //bypass StreamValueCommands to implement our own way

	Array<uint8> data;
	for (auto &i : customValuesManager->items) data.add((int) i->getLinkedValue(multiplexIndex));
	streamingModule->sendBytes(data, getCustomParams(multiplexIndex));
}

void SendStreamRawDataCommand::customValueCreated(Parameter * p)
{
	if (IntParameter* ip = dynamic_cast<IntParameter*>(p))
	{
		ip->hexMode = true;
		ip->setRange(0, 255);
	}
}
