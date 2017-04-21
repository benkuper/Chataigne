/*
  ==============================================================================

    LiveOSCDeviceCommand.cpp
    Created: 13 Apr 2017 11:36:24am
    Author:  Ben

  ==============================================================================
*/

#include "LiveOSCDeviceCommand.h"



LiveOSCDeviceCommand::LiveOSCDeviceCommand(LiveOSCModule * _module, CommandContext context, var params) :
	OSCCommand(_module, context, params)
{
	deviceID = argumentsContainer.addIntParameter("DeviceID ID", "id of the device", 0, 0, 1000);
	paramID = argumentsContainer.addIntParameter("Param ID", "id of the parameter", 0, 0, 1000);
	value = argumentsContainer.addFloatParameter("Param value", "Value of the target parameter", 0,0,1);
	toggleOnTrigger = addBoolParameter("Toggle", "Toggle on trigger", false);

	setTargetMappingParameterAt(value, 0);
}

LiveOSCDeviceCommand::~LiveOSCDeviceCommand()
{
}

void LiveOSCDeviceCommand::trigger()
{
	OSCCommand::trigger();
	if (toggleOnTrigger->boolValue()) value->setValue(value->floatValue() == 1 ? 0 : 1);
}
