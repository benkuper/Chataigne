/*
  ==============================================================================

    LiveOSCSceneCommand.h
    Created: 13 Apr 2017 11:02:54am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../commands/OSCCommand.h"
#include "../LiveOSCModule.h"

class LiveOSCSceneCommand :
	public OSCCommand
{
public:
	LiveOSCSceneCommand(LiveOSCModule * _module, CommandContext context, var params);
	~LiveOSCSceneCommand();

	LiveOSCModule * liveModule;

	IntParameter * targetScene;
	BoolParameter * autoNextScene;
	Trigger * resetScene;

	void triggerInternal() override;

	void onContainerTriggerTriggered(Trigger * t) override;

	static LiveOSCSceneCommand * create(ControllableContainer * module, CommandContext context, var params) { return new LiveOSCSceneCommand((LiveOSCModule *)module, context, params); }

};