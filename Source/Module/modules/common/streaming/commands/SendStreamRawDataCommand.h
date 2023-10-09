/*
  ==============================================================================

    SendTCPRawDataCommand.h
    Created: 5 Jan 2018 12:40:28am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class SendStreamRawDataCommand :
	public SendStreamValuesCommand
{
public:
	SendStreamRawDataCommand(StreamingModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	virtual ~SendStreamRawDataCommand();

	void triggerInternal(int multiplexIndex) override;

	void customValueCreated(Parameter * p);

	static SendStreamRawDataCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new SendStreamRawDataCommand((StreamingModule *)module, context, params, multiplex); }
};
