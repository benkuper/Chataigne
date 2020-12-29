/*
  ==============================================================================

    StreamDeckCommand.h
    Created: 23 Nov 2019 5:21:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../StreamDeckModule.h"

class StreamDeckCommand :
	public BaseCommand
{
public:
	enum StreamDeckAction { SET_COLOR, SET_IMAGE, SET_ALL_COLOR, SET_BRIGHTNESS};

	StreamDeckCommand(StreamDeckModule* _module, CommandContext context, var params, Multiplex* multiplex = nullptr);
	~StreamDeckCommand();

	StreamDeckAction action;
	StreamDeckModule* streamDeckModule;

	IntParameter* row;
	IntParameter* column;
	Parameter * valueParam;

	void triggerInternal(int multiplexIndex) override;

	static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex * multiplex) { return new StreamDeckCommand((StreamDeckModule*)module, context, params, multiplex); }


};