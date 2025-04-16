/*
  ==============================================================================

    ProcessorManagerUI.h
    Created: 15 Oct 2017 1:26:01pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ProcessorManagerUI :
	public ManagerUI<ProcessorManager, Processor, ProcessorUI>
{
public:

	ProcessorManagerUI(ProcessorManager *_manager, bool useViewport = true);
	~ProcessorManagerUI();

	void itemDropped(const SourceDetails &details) override;
	virtual void addItemFromMenu(Processor * item, bool isFromAddButton, Point<int> mouseDownPos) override;
	ProcessorUI * createUIForItem(Processor *) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorManagerUI)
};