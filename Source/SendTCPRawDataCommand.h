/*
  ==============================================================================

    SendTCPRawDataCommand.h
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "BaseCommand.h"
#include "TCPModule.h"

class SendTCPRawDataCommand :
	public BaseCommand
{
public:
	SendTCPRawDataCommand(TCPModule * _module, CommandContext context, var params);
	~SendTCPRawDataCommand();

	TCPModule * tcpModule;

	IntParameter * numBytes;
	ControllableContainer dataContainer;
	Array<IntParameter *> bytes;

	void updateBytesParams();
	void onContainerParameterChangedAsync(Parameter * p, const var &param) override;
	void trigger() override;

	static SendTCPRawDataCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendTCPRawDataCommand((TCPModule *)module, context, params); }

};