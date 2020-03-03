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
	customValuesManager->addBaseManagerListener(this);
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

void SendStreamRawDataCommand::itemAdded(CustomValuesCommandArgument* item)
{
	SendStreamValuesCommand::itemAdded(item);
	IntParameter* p = dynamic_cast<IntParameter*>(item->param);
	if (p != nullptr)
	{
		p->hexMode = true;
		p->setRange(0, 255);
	}
}
