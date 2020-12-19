/*
  ==============================================================================

    IteratorUI.h
    Created: 19 Dec 2020 12:12:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Iterator.h"
#include "../../ui/ProcessorManagerUI.h"

class IteratorUI :
	public ProcessorUI,
	public ProcessorManagerUI::ManagerUIListener
{
public:
	IteratorUI(IteratorProcessor * it);
	virtual ~IteratorUI();

	IteratorProcessor* iterator;
	ProcessorManagerUI processorManagerUI;

	void resizedInternalContent(Rectangle<int>& r) override;

	void updateProcessorManagerBounds();

	void itemUIAdded(ProcessorUI* pui) override;
	void itemUIRemoved(ProcessorUI* pui) override;

	void childBoundsChanged(Component* c) override;
};

