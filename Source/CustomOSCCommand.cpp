/*
  ==============================================================================

    CustomOSCCommand.cpp
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

#include "CustomOSCCommand.h"

CustomOSCCommand::CustomOSCCommand(CustomOSCOutput * output, CommandContext context, var params) :
	OSCCommand(output, context, params)
{
}

CustomOSCCommand::~CustomOSCCommand()
{
}
