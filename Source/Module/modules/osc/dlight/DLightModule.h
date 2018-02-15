/*
  ==============================================================================

    DLightModule.h
    Created: 21 Oct 2017 3:33:05pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "../OSCModule.h"

class DLightModule :
	public OSCModule
{
public:
	DLightModule();
	~DLightModule() {}

	static DLightModule * create() { return new DLightModule(); }
	virtual String getDefaultTypeString() const override { return "DLight"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DLightModule)
};


