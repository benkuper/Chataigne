/*
  ==============================================================================

    VLCModule.h
    Created: 21 Oct 2017 5:04:44pm
    Author:  Ben

  ==============================================================================
*/

#pragma once


#pragma once
#include "TCPModule.h"

class VLCModule :
	public TCPModule
{
public:
	VLCModule();
	~VLCModule() {}

	static VLCModule * create() { return new VLCModule(); }
	virtual String getDefaultTypeString() const override { return "VLC"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VLCModule)
};