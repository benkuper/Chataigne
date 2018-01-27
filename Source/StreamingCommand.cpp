/*
  ==============================================================================

    StreamingCommand.cpp
    Created: 27 Jan 2018 12:22:20pm
    Author:  Ben

  ==============================================================================
*/

#include "StreamingCommand.h"

StreamingCommand::StreamingCommand(StreamingModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	streamingModule(_module)
{

}

StreamingCommand::~StreamingCommand()
{

}