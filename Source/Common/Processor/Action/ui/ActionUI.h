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
	public Action::AsyncListener,
	public ConsequenceManager::AsyncListener,
	public UITimerTarget
{
public:
	ActionUI(Action*, bool showMiniModeBT = false);
	virtual ~ActionUI();

	Action* action;
	std::unique_ptr<TriggerButtonUI> triggerUI;
	std::unique_ptr<FloatSliderUI> progressionUI;
	std::unique_ptr<FloatSliderUI> staggerUI;

	virtual void paint(Graphics& g) override;

	void controllableFeedbackUpdateInternal(Controllable* c) override;

	void updateStaggerUI();

	virtual void resizedInternalHeader(Rectangle<int>& r) override;
	virtual void paintOverChildren(Graphics& g) override;

	virtual void itemDropped(const SourceDetails& details) override;

	void newMessage(const Action::ActionEvent& e) override;
	void newMessage(const ConsequenceManager::ConsequenceManagerEvent& e) override;

	void handlePaintTimerInternal();

	virtual void addContextMenuItems(PopupMenu& p) override;
	virtual void handleContextMenuResult(int result) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ActionUI)
};