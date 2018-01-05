/*
  ==============================================================================

    SendUDPRawDataCommand.h
    Created: 5 Jan 2018 12:40:38am
    Author:  Ben

  ==============================================================================
*/


#pragma once

#include "BaseCommand.h"
#include "UDPModule.h"

class SendUDPRawDataCommand :
	public BaseCommand
{
public:
	SendUDPRawDataCommand(UDPModule * _module, CommandContext context, var params);
	~SendUDPRawDataCommand();

	UDPModule * udpModule;

	IntParameter * numBytes;
	ControllableContainer dataContainer;
	Array<IntParameter *> bytes;

	void updateBytesParams();

	void onContainerParameterChanged(Parameter * p) override;
	void trigger() override;

	static SendUDPRawDataCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendUDPRawDataCommand((UDPModule *)module, context, params); }

};