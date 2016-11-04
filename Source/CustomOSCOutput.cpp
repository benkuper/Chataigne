/*
  ==============================================================================

    CustomOSCOutput.cpp
    Created: 29 Oct 2016 7:20:37pm
    Author:  bkupe

  ==============================================================================
*/

#include "CustomOSCOutput.h"
#include "CustomOSCCommand.h"
#include "CommandFactory.h"

CustomOSCOutput::CustomOSCOutput() :
	OSCOutput("OSC")
{
	commandDefs.add(new CommandDefinition(this, "", "Custom Message", &CustomOSCCommand::create));
}

CustomOSCOutput::~CustomOSCOutput()
{
}
