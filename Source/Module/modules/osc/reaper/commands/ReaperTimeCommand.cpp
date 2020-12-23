/*
  ==============================================================================

    ReaperTimeCommand.cpp
    Created: 26 Jan 2017 2:42:39pm
    Author:  Ben

  ==============================================================================
*/

#include "ReaperTimeCommand.h"

ReaperTimeCommand::ReaperTimeCommand(ReaperModule * _module, CommandContext context, var params, IteratorProcessor * iterator) :
	OSCCommand(_module,context,params),
	reaperModule(_module)
{
	timeParam = argumentsContainer.addFloatParameter("Time", "Target time on the Reaper timeline", 0,0, 100000);
	timeParam->defaultUI = FloatParameter::TIME;
	//addTargetMappingParameterAt(timeParam,0);

	stopTimePlay = addBoolParameter("Stop/Time/Play", "Sends a Stop before, then time and then play", true);
}

ReaperTimeCommand::~ReaperTimeCommand()
{
}

void ReaperTimeCommand::triggerInternal()
{
	if (stopTimePlay->boolValue()) oscModule->sendOSC(OSCMessage("/stop"));
	OSCCommand::triggerInternal();
	if (stopTimePlay->boolValue()) oscModule->sendOSC(OSCMessage("/play"));
	
}
