/*
  ==============================================================================

    ActionUI.h
    Created: 28 Oct 2016 8:05:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ActionUI :
	public ProcessorUI,
	public Action::AsyncListener
{
public:
	ActionUI(Action *);
	virtual ~ActionUI();

	Action* action;
	std::unique_ptr<TriggerButtonUI> triggerUI;
	std::unique_ptr<FloatSliderUI> progressionUI;

	void paint(Graphics &g) override;

	void updateBGColor() override;

	void controllableFeedbackUpdateInternal(Controllable * c) override;

	void resizedInternalHeader(Rectangle<int> &r) override;
	void paintOverChildren(Graphics &g) override;

	void itemDropped(const SourceDetails &details) override;

	void newMessage(const Action::ActionEvent &e) override;

	virtual void addContextMenuItems(PopupMenu& p) override;
	virtual void handleContextMenuResult(int result) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ActionUI)
};

