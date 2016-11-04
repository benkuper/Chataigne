/*
  ==============================================================================

    CustomOSCOutput.h
    Created: 29 Oct 2016 7:20:37pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef CUSTOMOSCOUTPUT_H_INCLUDED
#define CUSTOMOSCOUTPUT_H_INCLUDED

#include "OSCOutput.h"

class CustomOSCOutput :
	public OSCOutput
{
public:
	CustomOSCOutput();
	virtual ~CustomOSCOutput();

	static CustomOSCOutput * create() { return new CustomOSCOutput(); }
	virtual String getTypeString() const { return "CustomOSC"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(CustomOSCOutput)
};



#endif  // CUSTOMOSCOUTPUT_H_INCLUDED
