/*
  ==============================================================================

    LiveOSCCommandBase.h
    Created: 13 Apr 2017 11:28:34am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../commands/OSCCommand.h"
#include "../LiveOSCModule.h"

class LiveOSCCommandBase :
	public OSCCommand
{
public:
	LiveOSCCommandBase(LiveOSCModule * _module, CommandContext context, var params);
	~LiveOSCCommandBase();

	LiveOSCModule * liveModule;

	enum ActionType {ARM, MUTE, PAN, SOLO, VOLUME, SEND, TRACK_STOP, CLIP_PLAY, CLIP_STOP, DEVICE_ENABLE, DEVICE_PARAM};
	ActionType type;

	enum ValueType {NONE, FLOAT, BOOL };
	ValueType valueType;

	String liveCommand;

	//Target
	EnumParameter * trackTarget;

	//Track / Return
	IntParameter * trackID;

	//Clip
	IntParameter * clipID;

	//Device
	IntParameter * deviceID;

	//Send
	IntParameter * sendID;

	//Parameter
	IntParameter * parameterID;

	//Values
	Parameter * value;

	void rebuildAddress() override;

	void onContainerParameterChanged(Parameter * p) override;

	static LiveOSCCommandBase * create(ControllableContainer * module, CommandContext context, var params) { return new LiveOSCCommandBase((LiveOSCModule *)module, context, params); }

};