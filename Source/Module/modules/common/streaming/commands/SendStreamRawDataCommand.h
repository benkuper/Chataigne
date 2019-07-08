/*
  ==============================================================================

    SendTCPRawDataCommand.h
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "SendStreamValuesCommand.h"

class SendStreamRawDataCommand :
	public SendStreamValuesCommand,
	public CustomValuesCommandArgumentManager::ManagerListener
{
public:
	SendStreamRawDataCommand(StreamingModule * _module, CommandContext context, var params);
	~SendStreamRawDataCommand();

	void triggerInternal() override;

	void itemAdded(CustomValuesCommandArgument* item) override;

	static SendStreamRawDataCommand * create(ControllableContainer * module, CommandContext context, var params) { return new SendStreamRawDataCommand((StreamingModule *)module, context, params); }
};
