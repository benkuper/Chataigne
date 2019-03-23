/*
  ==============================================================================

    LiveOSCCommand.cpp
    Created: 13 Apr 2017 11:02:54am
    Author:  Ben

  ==============================================================================
*/

#include "LiveOSCSceneCommand.h"


LiveOSCSceneCommand::LiveOSCSceneCommand(LiveOSCModule * _module, CommandContext context, var params) :
	OSCCommand(_module, context, params),
	liveModule(_module)
{
	addressModel = "/live/scene/play";
	rebuildAddress();

	targetScene = argumentsContainer.addIntParameter("Scene", "Index of the scene to play", 0, 0, 1000);
	autoNextScene = addBoolParameter("Auto Next", "If enabled, when triggered, scene will increment", false);
	resetScene = addTrigger("Reset", "Reset to scene 1");
}

LiveOSCSceneCommand::~LiveOSCSceneCommand()
{
}

void LiveOSCSceneCommand::triggerInternal()
{
	OSCCommand::triggerInternal();
	if (autoNextScene->boolValue()) targetScene->setValue(targetScene->intValue() + 1);
}

void LiveOSCSceneCommand::onContainerTriggerTriggered(Trigger * t)
{
	if (t == resetScene)
	{ 
		targetScene->setValue(0);
		oscModule->sendOSC(OSCMessage("/live/stop"));
	}
}
