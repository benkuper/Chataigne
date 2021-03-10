/*
  ==============================================================================

    StreamingCommand.cpp
    Created: 27 Jan 2018 12:22:20pm
    Author:  Ben

  ==============================================================================
*/

StreamingCommand::StreamingCommand(StreamingModule * _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	streamingModule(_module)
{

}

StreamingCommand::~StreamingCommand()
{

}