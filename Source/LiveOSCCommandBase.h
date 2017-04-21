/*
  ==============================================================================

    LiveOSCCommandBase.h
    Created: 13 Apr 2017 11:28:34am
    Author:  Ben

  ==============================================================================
*/

#ifndef LIVEOSCCOMMANDBASE_H_INCLUDED
#define LIVEOSCCOMMANDBASE_H_INCLUDED


#include "OSCCommand.h"
#include "LiveOSCModule.h"

class LiveOSCCommandBase :
	public OSCCommand
{
public:
	LiveOSCCommandBase(LiveOSCModule * _module, CommandContext context, var params);
	~LiveOSCCommandBase();

	LiveOSCModule * liveModule;

	IntParameter * trackID;
	FloatParameter * value;

	BoolParameter * toggle;
	BoolParameter * autoNext;

	Trigger * muteAllTracks;

	void trigger() override;

	void onContainerTriggerTriggered(Trigger * t) override;

	static LiveOSCCommandBase * create(ControllableContainer * module, CommandContext context, var params) { return new LiveOSCCommandBase((LiveOSCModule *)module, context, params); }

};



#endif  // LIVEOSCCOMMANDBASE_H_INCLUDED
