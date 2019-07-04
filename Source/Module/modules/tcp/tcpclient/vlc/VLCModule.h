/*
  ==============================================================================

    VLCModule.h
    Created: 21 Oct 2017 5:04:44pm
    Author:  Ben

  ==============================================================================
*/

#pragma once


#pragma once
#include "../TCPClientModule.h"

class VLCModule :
	public TCPClientModule
{
public:
	VLCModule();
	~VLCModule() {}

	static VLCModule * create() { return new VLCModule(); }
	virtual String getDefaultTypeString() const override { return "VLC"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VLCModule)
};