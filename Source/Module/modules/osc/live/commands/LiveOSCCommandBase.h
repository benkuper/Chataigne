/*
  ==============================================================================

    LiveOSCCommandBase.h
    Created: 13 Apr 2017 11:28:34am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class LiveOSCCommandBase :
	public OSCCommand
{
public:
	LiveOSCCommandBase(LiveOSCModule * _module, CommandContext context, var params, Multiplex * multiplex = nullptr);
	~LiveOSCCommandBase();

	LiveOSCModule* liveModule; 
	String addressModelBase;

	IntParameter* sceneID;
	IntParameter* trackID;
	IntParameter* clipID;
	IntParameter* deviceID;
	IntParameter* paramID;

	Parameter* value;
	Parameter* arg;


	static LiveOSCCommandBase * create(ControllableContainer * module, CommandContext context, var params, Multiplex * multiplex) { return new LiveOSCCommandBase((LiveOSCModule *)module, context, params, multiplex); }

};