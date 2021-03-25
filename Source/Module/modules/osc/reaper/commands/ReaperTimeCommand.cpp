/*
  ==============================================================================

    ReaperTimeCommand.cpp
    Created: 26 Jan 2017 2:42:39pm
    Author:  Ben

  ==============================================================================
*/

ReaperTimeCommand::ReaperTimeCommand(ReaperModule * _module, CommandContext context, var params, Multiplex * multiplex) :
	OSCCommand(_module,context,params),
	reaperModule(_module)
{
	timeParam = argumentsContainer.addFloatParameter("Time", "Target time on the Reaper timeline", 0,0, 100000);
	timeParam->defaultUI = FloatParameter::TIME;
	argumentsContainer.linkParamToMappingIndex(timeParam,0);

	stopTimePlay = addBoolParameter("Stop/Time/Play", "Sends a Stop before, then time and then play", true);
}

ReaperTimeCommand::~ReaperTimeCommand()
{
}

void ReaperTimeCommand::triggerInternal(int multiplexIndex)
{
	if (stopTimePlay->boolValue()) oscModule->sendOSC(OSCMessage("/stop"));
	OSCCommand::triggerInternal(multiplexIndex);
	if (stopTimePlay->boolValue()) oscModule->sendOSC(OSCMessage("/play"));
	
}
