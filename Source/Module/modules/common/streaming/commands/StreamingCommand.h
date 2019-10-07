/*
  ==============================================================================

    StreamingCommand.h
    Created: 27 Jan 2018 12:22:20pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../StreamingModule.h"
#include "Common/Command/BaseCommand.h"

class StreamingCommand :
	public BaseCommand
{
public:
	StreamingCommand(StreamingModule * _module, CommandContext context, var params);
	~StreamingCommand();

	StreamingModule * streamingModule;
};