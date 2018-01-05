/*
  ==============================================================================

    Processor.h
    Created: 15 Oct 2017 1:24:47pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class ProcessorUI;

class Processor :
	public BaseItem
{
public:
	enum ProcessorType { ACTION, MAPPING };
	
	Processor(const String &name = "Processor", bool canBeDisabled = true);
	virtual ~Processor();

	ProcessorType type;
	
	bool forceDisabled;

	virtual void setForceDisabled(bool value, bool force = false);

	//UI
	virtual ProcessorUI * getUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Processor)
};