/*
  ==============================================================================

    CustomOSCCommand.cpp
    Created: 3 Nov 2016 12:41:23pm
    Author:  bkupe

  ==============================================================================
*/

#include "CustomOSCCommand.h"
#include "CustomOSCCommandEditor.h"

CustomOSCCommand::CustomOSCCommand(CustomOSCModule * module, CommandContext context, var params) :
	OSCCommand(module, context, params)
{
}

CustomOSCCommand::~CustomOSCCommand()
{
}

InspectableEditor * CustomOSCCommand::getEditor(bool)
{
	return new CustomOSCCommandEditor(this, false);
}
