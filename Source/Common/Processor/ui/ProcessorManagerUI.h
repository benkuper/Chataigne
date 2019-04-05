/*
  ==============================================================================

    ProcessorManagerUI.h
    Created: 15 Oct 2017 1:26:01pm
    Author:  Ben

  ==============================================================================
*/

#pragma once


#include "ProcessorUI.h"
#include "../ProcessorManager.h"

class ProcessorManagerUI :
	public BaseManagerUI<ProcessorManager, Processor, ProcessorUI>
{
public:

	ProcessorManagerUI(ProcessorManager *_manager);
	~ProcessorManagerUI();

	void itemDropped(const SourceDetails &details) override;

	ProcessorUI * createUIForItem(Processor *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorManagerUI)
};