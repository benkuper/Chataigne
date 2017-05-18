/*
  ==============================================================================

    MilluminModule.h
    Created: 17 May 2017 7:41:53pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "OSCModule.h"

class MilluminModule :
	public OSCModule
{
public:
	MilluminModule();
	~MilluminModule() {}

	static MilluminModule * create() { return new MilluminModule(); }
	virtual String getDefaultTypeString() const override { return "Millumin"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MilluminModule)
};


