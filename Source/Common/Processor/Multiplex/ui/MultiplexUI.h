/*
  ==============================================================================

    MultiplexUI.h
    Created: 19 Dec 2020 12:12:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Multiplex.h"
#include "../../ui/ProcessorManagerUI.h"

class MultiplexUI :
	public ProcessorUI,
	public ProcessorManagerUI::ManagerUIListener
{
public:
	MultiplexUI(Multiplex * it);
	virtual ~MultiplexUI();

	Multiplex* multiplex;
	ProcessorManagerUI processorManagerUI;

	std::unique_ptr<IntStepperUI> previewUI;

	void resizedInternalHeader(Rectangle<int>& r) override;
	void resizedInternalContent(Rectangle<int>& r) override;

	void updateProcessorManagerBounds();

	void itemUIAdded(ProcessorUI* pui) override;
	void itemUIRemoved(ProcessorUI* pui) override;

	void childBoundsChanged(Component* c) override;
};

