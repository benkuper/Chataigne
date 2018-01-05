/*
  ==============================================================================

    SendTCPRawDataCommand.cpp
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamRawDataCommand.h"

SendStreamRawDataCommand::SendStreamRawDataCommand(StreamingModule * _module, CommandContext context, var params) :
	BaseCommand(_module,context, params),
	streamingModule(_module),
	dataContainer("Bytes")
{
	numBytes = addIntParameter("Num bytes", "Number of bytes to send", 1, 1, 512);
	addChildControllableContainer(&dataContainer);
	updateBytesParams();
}

SendStreamRawDataCommand::~SendStreamRawDataCommand()
{
}

void SendStreamRawDataCommand::updateBytesParams()
{
	
	while (dataContainer.controllables.size() > numBytes->intValue())
	{
		dataContainer.controllables[dataContainer.controllables.size() - 1]->remove();
	}

	while (dataContainer.controllables.size() < numBytes->intValue())
	{
		String index = String(dataContainer.controllables.size());
		dataContainer.addIntParameter("#" + index, "Data for the byte #" + index, 0, 0, 255);
	}

}

void SendStreamRawDataCommand::onContainerParameterChangedAsync(Parameter * p, const var &)
{
	if (p == numBytes)
	{
		updateBytesParams();
	}
}

void SendStreamRawDataCommand::trigger()
{
	Array<uint8> data;
	for (auto &c : dataContainer.controllables) data.add(((IntParameter *)c)->intValue());
	streamingModule->sendBytes(data);
}
