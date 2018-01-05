/*
  ==============================================================================

    SendTCPRawDataCommand.cpp
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#include "SendTCPRawDataCommand.h"

SendTCPRawDataCommand::SendTCPRawDataCommand(TCPModule * _module, CommandContext context, var params) :
	BaseCommand(_module,context, params),
	tcpModule(_module),
	dataContainer("Bytes")
{
	numBytes = addIntParameter("Num bytes", "Number of bytes to send", 1, 1, 512);
	addChildControllableContainer(&dataContainer);
	updateBytesParams();
}

SendTCPRawDataCommand::~SendTCPRawDataCommand()
{
}

void SendTCPRawDataCommand::updateBytesParams()
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

void SendTCPRawDataCommand::onContainerParameterChangedAsync(Parameter * p, const var &)
{
	if (p == numBytes)
	{
		updateBytesParams();
	}
}

void SendTCPRawDataCommand::trigger()
{
	Array<uint8> data;
	for (auto &c : dataContainer.controllables) data.add(((IntParameter *)c)->intValue());
	tcpModule->sendRawData(data);
}
