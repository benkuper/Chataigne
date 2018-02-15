/*
  ==============================================================================

    SendTCPRawDataCommand.cpp
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#include "SendStreamRawDataCommand.h"

SendStreamRawDataCommand::SendStreamRawDataCommand(StreamingModule * _module, CommandContext context, var params) :
	StreamingCommand(_module,context, params),
	dataContainer("Bytes"),
	targetMappingByteIndex(nullptr)
{
	numBytes = addIntParameter("Num bytes", "Number of bytes to send", 1, 1, 512);
	addChildControllableContainer(&dataContainer);

	if (context == CommandContext::MAPPING)
	{
		targetMappingByteIndex = addIntParameter("Map to byte #", "The index of the byte to use as mapping output, -1 = none", -1, -1, 0);
	}

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

	if (context == CommandContext::MAPPING) targetMappingByteIndex->setRange(-1, numBytes->intValue() - 1);

}

void SendStreamRawDataCommand::onContainerParameterChangedAsync(Parameter * p, const var &)
{
	if (p == numBytes)
	{
		updateBytesParams();
	} else if (p == targetMappingByteIndex)
	{
		clearTargetMappingParameters();
		if (targetMappingByteIndex->intValue() >= 0 && targetMappingByteIndex->intValue() < numBytes->intValue()) setTargetMappingParameterAt((Parameter *)dataContainer.controllables[targetMappingByteIndex->intValue()],0);
	}
}

void SendStreamRawDataCommand::trigger()
{
	StreamingCommand::trigger();

	Array<uint8> data;
	for (auto &c : dataContainer.controllables) data.add(((IntParameter *)c)->intValue());
	streamingModule->sendBytes(data);
}
