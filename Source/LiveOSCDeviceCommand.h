/*
  ==============================================================================

    LiveOSCDeviceCommand.h
    Created: 13 Apr 2017 11:36:24am
    Author:  Ben

  ==============================================================================
*/

#ifndef LIVEOSCDEVICECOMMAND_H_INCLUDED
#define LIVEOSCDEVICECOMMAND_H_INCLUDED


#include "OSCCommand.h"
#include "LiveOSCModule.h"

class LiveOSCDeviceCommand :
	public OSCCommand
{
public:
	LiveOSCDeviceCommand(LiveOSCModule * _module, CommandContext context, var params);
	~LiveOSCDeviceCommand();

	LiveOSCModule * liveModule;

	IntParameter * deviceID;
	IntParameter * paramID;
	FloatParameter * value;
	BoolParameter * toggleOnTrigger;

	void trigger() override;

	static LiveOSCDeviceCommand * create(ControllableContainer * module, CommandContext context, var params) { return new LiveOSCDeviceCommand((LiveOSCModule *)module, context, params); }

};





#endif  // LIVEOSCDEVICECOMMAND_H_INCLUDED
