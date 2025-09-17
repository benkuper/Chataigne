/*
  ==============================================================================

    MultiplexUI.h
    Created: 19 Dec 2020 12:12:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class MultiplexUI :
	public ProcessorUI,
	public ProcessorManagerUI::ManagerUIListener, 
	public ProcessorUI::ProcessorUIListener
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

	void BaseItemUIAdded(ProcessorUI* pui) override;
	void BaseItemUIRemoved(ProcessorUI* pui) override;

	void childBoundsChanged(Component* c) override;

	void processorAskForFocus(ProcessorUI* pui) override;
};

