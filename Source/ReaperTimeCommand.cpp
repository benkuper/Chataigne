/*
  ==============================================================================

    ReaperTimeCommand.cpp
    Created: 26 Jan 2017 2:42:39pm
    Author:  Ben

  ==============================================================================
*/

#include "ReaperTimeCommand.h"

ReaperTimeCommand::ReaperTimeCommand(ReaperModule * _module, CommandContext context, var params) :
	OSCCommand(_module,context,params),
	reaperModule(_module)
{
	timeParam = argumentsContainer.addFloatParameter("Time", "Target time on the Reaper timeline", 0,0, 100000);
	timeParam->defaultUI = FloatParameter::TIME;
	setTargetMappingParameter(timeParam);
}

ReaperTimeCommand::~ReaperTimeCommand()
{
}
