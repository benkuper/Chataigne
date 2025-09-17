/*
  ==============================================================================

    ConductorUI.h
    Created: 5 Oct 2021 9:40:58pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


class ConductorUI :
	public ActionUI,
	public ProcessorManagerUI::ManagerUIListener,
	public ProcessorUI::ProcessorUIListener
{
public:
	ConductorUI(Conductor* conductor);
	virtual ~ConductorUI();

	Conductor* conductor;
	ProcessorManagerUI processorManagerUI;

	std::unique_ptr<IntStepperUI> cueUI;
	std::unique_ptr<StringParameterUI> nextCueUI;

	void updateBGColor() override;

	void resizedInternalHeader(Rectangle<int>& r) override;
	void resizedInternalContent(Rectangle<int>& r) override;

	void updateProcessorManagerBounds();

	void ItemUIAdded(ProcessorUI* pui) override;
	void ItemUIRemoved(ProcessorUI* pui) override;

	void childBoundsChanged(Component* c) override;

	void processorAskForFocus(ProcessorUI* pui) override;

	void newMessage(const Action::ActionEvent& e) override;
};

