/*
  ==============================================================================

    SerialDeviceParameter.h
    Created: 15 Mar 2017 10:14:56am
    Author:  Ben-Portable

  ==============================================================================
*/

#ifndef SERIALDEVICEPARAMETER_H_INCLUDED
#define SERIALDEVICEPARAMETER_H_INCLUDED

#include "StringParameter.h"

class SerialDeviceParameter :
	public StringParameter
{
public:
	SerialDeviceParameter(const String &name, const String &description, const String &initialValue, bool enabled);
	~SerialDeviceParameter();


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SerialDeviceParameter)
};


#endif  // SERIALDEVICEPARAMETER_H_INCLUDED
