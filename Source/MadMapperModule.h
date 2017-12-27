/*
  ==============================================================================

    MadMapperModule.h
    Created: 10 Dec 2017 12:08:33pm
    Author:  Ben

  ==============================================================================
*/

#pragma once



#pragma once
#include "OSCModule.h"

class MadMapperModule :
	public OSCModule
{
public:
	MadMapperModule();
	~MadMapperModule() {}

	static MadMapperModule * create() { return new MadMapperModule(); }
	virtual String getDefaultTypeString() const override { return "MadMapper"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MadMapperModule)
};

