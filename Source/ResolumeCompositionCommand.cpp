/*
  ==============================================================================

    ResolumeCompositionCommand.cpp
    Created: 3 Nov 2016 12:33:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "ResolumeCompositionCommand.h"

ResolumeCompositionCommand::ResolumeCompositionCommand(ResolumeModule * module, CommandContext context, var params) :
	OSCCommand(module,context,params)
{
}

ResolumeCompositionCommand::~ResolumeCompositionCommand()
{
}
