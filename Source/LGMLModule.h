/*
  ==============================================================================

    LGMLModule.h
    Created: 17 May 2017 6:19:54pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "OSCModule.h"

class LGMLModule :
	public OSCModule
{
public:
	LGMLModule();
	~LGMLModule() {}

	static LGMLModule * create() { return new LGMLModule(); }
	virtual String getDefaultTypeString() const override { return "LeGrandMechantLoop"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LGMLModule)
};