/*
  ==============================================================================

    ReaperModule.h
    Created: 26 Jan 2017 2:39:33pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ReaperModule :
	public OSCModule
{
public:
	ReaperModule();
	~ReaperModule() {}

	static ReaperModule * create() { return new ReaperModule(); }
	virtual String getDefaultTypeString() const override { return "Reaper"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ReaperModule)
};