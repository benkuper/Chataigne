/*
  ==============================================================================

    SendTCPStringCommand.h
    Created: 21 Oct 2017 5:05:39pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "StreamingCommand.h"

class SendStreamStringCommand :
	public StreamingCommand
{
public:
	SendStreamStringCommand(StreamingModule * _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~SendStreamStringCommand();

	enum DataMode { STRING, HEX };
	DataMode dataMode;
	
	BoolParameter * appendCR;
	BoolParameter * appendNL;

	StringParameter * prefix;
	StringParameter * valueParam;
	
	Array<uint8> mappedValues;

	void setValue(var value, int multiplexIndex) override;
	void triggerInternal(int multiplexIndex) override;

	static SendStreamStringCommand * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new SendStreamStringCommand((StreamingModule *)module, context, params, multiplex); }

};
