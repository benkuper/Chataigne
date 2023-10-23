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
	public ConsequenceManager::AsyncListener
{
public:
	ActionUI(Action*, bool showMiniModeBT = false);
	virtual ~ActionUI();

	Action* action;
	std::unique_ptr<TriggerButtonUI> triggerUI;
	std::unique_ptr<FloatSliderUI> progressionUI;
	std::unique_ptr<FloatSliderUI> staggerUI;
	bool shouldRepaint;

	virtual void paint(Graphics& g) override;

	void controllableFeedbackUpdateInternal(Controllable* c) override;

	void updateStaggerUI();

	virtual void resizedInternalHeader(Rectangle<int>& r) override;
	virtual void paintOverChildren(Graphics& g) override;

	virtual void itemDropped(const SourceDetails& details) override;

	void newMessage(const Action::ActionEvent& e) override;
	void newMessage(const ConsequenceManager::ConsequenceManagerEvent& e) override;

	void handlePaintTimer();

	virtual void addContextMenuItems(PopupMenu& p) override;
	virtual void handleContextMenuResult(int result) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ActionUI)
};

class ActionUITimers :
	public Timer
{
public:
	juce_DeclareSingleton(ActionUITimers, true);
	ActionUITimers();
	~ActionUITimers() {}

	Array<ActionUI*, CriticalSection> actionsUI;

	void registerAction(ActionUI* ui);
	void unregisterAction(ActionUI* ui);
	void timerCallback();
};