/*
  ==============================================================================

    HeavyMModule.h
    Created: 7 Sep 2018 12:15:30am
    Author:  Ben

  ==============================================================================
*/

#pragma once



#pragma once
#include "../OSCModule.h"

class HeavyMModule :
	public OSCModule
{
public:
	HeavyMModule();
	~HeavyMModule() {}

	static HeavyMModule * create() { return new HeavyMModule(); }
	virtual String getDefaultTypeString() const override { return "Millumin"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(HeavyMModule)
};


