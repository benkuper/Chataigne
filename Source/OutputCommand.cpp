/*
  ==============================================================================

    OutputCommand.cpp
    Created: 3 Nov 2016 10:56:27am
    Author:  bkupe

  ==============================================================================
*/

#include "OutputCommand.h"
#include "Output.h"

OutputCommand::OutputCommand(Output * _output, CommandContext context, var params) :
	BaseCommand(_output, context, params),
	output(_output)
{

}

OutputCommand::~OutputCommand()
{
}
