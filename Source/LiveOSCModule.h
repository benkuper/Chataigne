/*
  ==============================================================================

    LiveOSCModule.h
    Created: 13 Apr 2017 11:02:29am
    Author:  Ben

  ==============================================================================
*/

#ifndef LIVEOSCMODULE_H_INCLUDED
#define LIVEOSCMODULE_H_INCLUDED

#include "OSCModule.h"

class LiveOSCModule :
	public OSCModule
{
public:
	LiveOSCModule();
	~LiveOSCModule() {}

	static LiveOSCModule * create() { return new LiveOSCModule(); }
	virtual String getDefaultTypeString() const override { return "LiveOSC2"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LiveOSCModule)
};



#endif  // LIVEOSCMODULE_H_INCLUDED
