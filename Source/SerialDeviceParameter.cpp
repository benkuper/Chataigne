/*
  ==============================================================================

    SerialDeviceParameter.cpp
    Created: 15 Mar 2017 10:14:56am
    Author:  Ben-Portable

  ==============================================================================
*/

#include "SerialDeviceParameter.h"

SerialDeviceParameter::SerialDeviceParameter(const String & name, const String & description, const String & initialValue, bool enabled) :
	StringParameter(name,description,initialValue,enabled)
{
}

SerialDeviceParameter::~SerialDeviceParameter()
{
}
