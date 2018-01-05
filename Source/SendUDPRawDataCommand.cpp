/*
  ==============================================================================

    SendUDPRawDataCommand.cpp
    Created: 5 Jan 2018 12:40:38am
    Author:  Ben

  ==============================================================================
*/

#include "SendUDPRawDataCommand.h"

SendUDPRawDataCommand::SendUDPRawDataCommand(UDPModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	udpModule(_module),
	dataContainer("Bytes")
{
	numBytes = addIntParameter("Num bytes", "Number of bytes to send", 1, 1, 512);
	addChildControllableContainer(&dataContainer);
	updateBytesParams();
}

SendUDPRawDataCommand::~SendUDPRawDataCommand()
{
}

void SendUDPRawDataCommand::updateBytesParams()
{
	while (dataContainer.controllables.size() < numBytes->intValue())
	{
		String index = String(dataContainer.controllables.size());
		dataContainer.addIntParameter("#" + index, "Data for the byte #" + index, 0, 0, 255);
	}

	while (dataContainer.controllables.size() > numBytes->intValue())
	{
		dataContainer.removeControllable(dataContainer.controllables[dataContainer.controllables.size() - 1]);
	}
}

void SendUDPRawDataCommand::onContainerParameterChanged(Parameter * p)
{
	if (p == numBytes)
	{
		updateBytesParams();
	}
}

void SendUDPRawDataCommand::trigger()
{
	Array<uint8> data;
	for (auto &c : dataContainer.controllables) data.add(((IntParameter *)c)->intValue());
	//udpModule->sendRawData(data);
}
