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
	liveModule(_module),
	trackTarget(nullptr),
	trackID(nullptr),
	value(nullptr)
{
	type = (ActionType)(int)params.getProperty("type", MUTE);

	float minVal = 0;
	float maxVal = 1;

	switch (type)
	{
	case ARM: liveCommand = "arm"; valueType = BOOL; break;
	case MUTE: liveCommand = "mute"; valueType = BOOL; break;
	case PAN: liveCommand = "panning"; valueType = FLOAT; minVal = -1; maxVal = 1; break;
	case SOLO: liveCommand = "solo"; valueType = BOOL; break;
	case VOLUME: liveCommand = "volume"; valueType = FLOAT; break;
	case SEND: liveCommand = "send"; valueType = FLOAT; maxVal = 127; break;
	case TRACK_STOP: liveCommand = "stop"; valueType = NONE; break;
	case CLIP_PLAY: liveCommand = "play"; valueType = NONE; break;
	case CLIP_STOP: liveCommand = "stop"; valueType = NONE; break;
	case DEVICE_ENABLE: liveCommand = "device/param"; valueType = BOOL; break;
	case DEVICE_PARAM: liveCommand = "device/param"; valueType = FLOAT; maxVal = 127; break;
	}

	if (type != CLIP_PLAY && type != CLIP_STOP)
	{
		trackTarget = addEnumParameter("Target", "Type of target : Master, Return or Track");
		if (type != MUTE && type != ARM && type != SOLO) trackTarget->addOption("Master", "master");
		trackTarget->addOption("Track", "track");
		if (type != ARM) trackTarget->addOption("Return", "return");

		if (type == ARM) trackTarget->hideInEditor = true;
	}

	trackID = argumentsContainer.addIntParameter("Track", "Index of the target track", 0, 0, 1000);
	if (trackTarget != nullptr) trackID->setEnabled(trackTarget->getValueData().toString() != "master");

	
	if (type == DEVICE_ENABLE || type == DEVICE_PARAM)
	{
		deviceID = argumentsContainer.addIntParameter("Device", "Index of the target device", 0, 0, 255);
		parameterID = argumentsContainer.addIntParameter("Parameter", "Index of the target parameter", 1, 0, 255);
		if (type == DEVICE_ENABLE) 
		{
			parameterID->setValue(0);
			parameterID->hideInEditor = true;
		}

	} else if (type == CLIP_PLAY || type == CLIP_STOP)
	{
		clipID = argumentsContainer.addIntParameter("Clip", "Index of the clip in the track", 0, 0, 255);
	} else if (type == SEND)
	{
		sendID = argumentsContainer.addIntParameter("Send", "Index of the send in the track", 0, 0, 255);
	}


	switch (valueType)
	{
	case NONE:	break;
	case FLOAT: value = argumentsContainer.addFloatParameter("Value", "Value", 0, minVal, maxVal); break;
	case BOOL: value = argumentsContainer.addBoolParameter("Value", "Value", false); break;
	}

	if(value != nullptr) addTargetMappingParameterAt(value, 0);
	rebuildAddressOnParamChanged = true;
	rebuildAddress();
}

LiveOSCCommandBase::~LiveOSCCommandBase()
{
}

void LiveOSCCommandBase::rebuildAddress()
{
	if (type == CLIP_STOP || type == CLIP_PLAY) addressModel = "/live/clip/" + liveCommand;
	else addressModel = "/live/" + trackTarget->getValueData().toString() + "/" + liveCommand;
	OSCCommand::rebuildAddress();
}

void LiveOSCCommandBase::onContainerParameterChanged(Parameter * p)
{
	OSCCommand::onContainerParameterChanged(p);
	if (p == trackTarget)
	{

		if (trackTarget != nullptr) trackID->setEnabled(trackTarget->getValueData().toString() != "master");
 	}
}
