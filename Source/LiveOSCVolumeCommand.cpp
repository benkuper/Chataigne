/*
  ==============================================================================

    LiveOSCVolumeCommand.cpp
    Created: 13 Apr 2017 11:27:49am
    Author:  Ben

  ==============================================================================
*/


#include "LiveOSCVolumeCommand.h"


LiveOSCVolumeCommand::LiveOSCVolumeCommand(LiveOSCModule * _module, CommandContext context, var params) :
	OSCCommand(_module, context, params),
	liveModule(_module)
{
	volume = argumentsContainer.addFloatParameter("Volume", "Volume of the target", .85f, 0, 1);
	increment = addFloatParameter("Increment", "Will increase or decrease the volume when trigger", 0,-1,1);
	resetVolume = addTrigger("Reset", "Reset to volume to default value (0.85)");

	setTargetMappingParameterAt(volume, 0);
}

LiveOSCVolumeCommand::~LiveOSCVolumeCommand()
{
}

void LiveOSCVolumeCommand::trigger()
{
	OSCCommand::trigger();
	volume->setValue(volume->floatValue() + increment->floatValue());
}

void LiveOSCVolumeCommand::onContainerTriggerTriggered(Trigger * t)
{
	if (t == resetVolume)
	{
		volume->setValue(.85f);
		trigger();
	}
}
