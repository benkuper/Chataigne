/*
  ==============================================================================

    ReaperModule.h
    Created: 26 Jan 2017 2:39:33pm
    Author:  Ben

  ==============================================================================
*/

#ifndef REAPERMODULE_H_INCLUDED
#define REAPERMODULE_H_INCLUDED

#include "OSCModule.h"

class ReaperModule :
	public OSCModule
{
public:
	ReaperModule();
	~ReaperModule() {}

	static ReaperModule * create() { return new ReaperModule(); }
	virtual String getTypeString() const override { return "Reaper"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReaperModule)
};



#endif  // REAPERMODULE_H_INCLUDED
