/*
  ==============================================================================

    ProcessorUI.h
    Created: 15 Oct 2017 1:25:44pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../Processor.h"

class ProcessorUI :
	public BaseItemUI<Processor>,
	public Timer
{
public:
	ProcessorUI(Processor *);
	virtual ~ProcessorUI();

	bool shouldRepaint;

	void timerCallback() override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorUI)
};