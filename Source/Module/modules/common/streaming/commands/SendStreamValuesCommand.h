/*
  ==============================================================================

    SendStreamValuesCommand.h
    Created: 27 Jan 2018 12:04:01pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class SendStreamValuesCommand :
	public StreamingCommand
{
public:
	SendStreamValuesCommand(StreamingModule * output, CommandContext context, var params, Multiplex* multiplex = nullptr);
	virtual ~SendStreamValuesCommand();

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void triggerInternal(int multiplexIndex) override;

	static SendStreamValuesCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new SendStreamValuesCommand((StreamingModule *)module, context, params, multiplex); }

};
