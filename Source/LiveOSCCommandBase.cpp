/*
  ==============================================================================

    LiveOSCCommandBase.cpp
    Created: 13 Apr 2017 11:28:34am
    Author:  Ben

  ==============================================================================
*/

#include "LiveOSCCommandBase.h"



LiveOSCCommandBase::LiveOSCCommandBase(LiveOSCModule * _module, CommandContext context, var params) :
	OSCCommand(_module, context, params),
	liveModule(_module)
{
	trackID = argumentsContainer.addIntParameter("Track", "Volume of the target", 0,0,1000);
	value = argumentsContainer.addFloatParameter("Value", "Value", 0, 0, 1);
	toggle = addBoolParameter("Toggle", "Toggle", false);
	autoNext = addBoolParameter("AutoNext", "autonext", false);
	muteAllTracks = addTrigger("Mute All0","Mute All");

	setTargetMappingParameterAt(value, 0);
}

LiveOSCCommandBase::~LiveOSCCommandBase()
{
}

void LiveOSCCommandBase::trigger()
{
	OSCCommand::trigger();
	if (toggle->boolValue()) value->setValue(value->floatValue() == 1 ? 0 : 1);
	if (autoNext->boolValue()) trackID->setValue(trackID->intValue() + 1);
}

void LiveOSCCommandBase::onContainerTriggerTriggered(Trigger * t)
{

	if (t == muteAllTracks)
	{
		for (int i = 0; i < 40; i++)
		{
			OSCMessage m("/live/mute");
			m.addInt32(i);
			m.addFloat32(1);
			oscModule->sendOSC(m);
		}
	}
	
}
