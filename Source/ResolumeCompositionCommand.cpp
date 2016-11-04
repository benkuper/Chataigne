/*
  ==============================================================================

    ResolumeCompositionCommand.cpp
    Created: 3 Nov 2016 12:33:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "ResolumeCompositionCommand.h"

ResolumeCompositionCommand::ResolumeCompositionCommand(ResolumeOutput * output, CommandContext context, var params) :
	OSCCommand(output,context,params)
{
}

ResolumeCompositionCommand::~ResolumeCompositionCommand()
{
}
