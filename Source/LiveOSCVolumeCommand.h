/*
  ==============================================================================

    LiveOSCVolumeCommand.h
    Created: 13 Apr 2017 11:27:49am
    Author:  Ben

  ==============================================================================
*/

#ifndef LiveOSCVolumeCommand_H_INCLUDED
#define LiveOSCVolumeCommand_H_INCLUDED


#include "OSCCommand.h"
#include "LiveOSCModule.h"

class LiveOSCVolumeCommand :
	public OSCCommand
{
public:
	LiveOSCVolumeCommand(LiveOSCModule * _module, CommandContext context, var params);
	~LiveOSCVolumeCommand();

	LiveOSCModule * liveModule;

	FloatParameter * volume;
	FloatParameter * increment;
	Trigger * resetVolume;

	void trigger() override;

	void onContainerTriggerTriggered(Trigger * t) override;

	static LiveOSCVolumeCommand * create(ControllableContainer * module, CommandContext context, var params) { return new LiveOSCVolumeCommand((LiveOSCModule *)module, context, params); }

};






#endif  // LiveOSCVolumeCommand_H_INCLUDED
