/*
  ==============================================================================

    PowerpointModule.h
    Created: 13 Jul 2018 12:11:43am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../OSCModule.h"

class PowerPointModule :
	public OSCModule
{
public:
	PowerPointModule();
	~PowerPointModule();

	IntParameter * currentSlide;

	static PowerPointModule * create() { return new PowerPointModule(); }
	virtual String getDefaultTypeString() const override { return "PowerPoint"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PowerPointModule)

};
