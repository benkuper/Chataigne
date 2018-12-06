/*
  ==============================================================================

    ActionUI.h
    Created: 28 Oct 2016 8:05:24pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../Action.h"
#include "../../ui/ProcessorUI.h"
 
class ActionUI :
	public ProcessorUI,
	public Action::AsyncListener
{
public:
	ActionUI(Action *);
	virtual ~ActionUI();

	Action * action;

	ScopedPointer<TriggerButtonUI> triggerAllUI;
	ScopedPointer<FloatSliderUI> progressionUI;

	void updateRoleBGColor();

	void controllableFeedbackUpdateInternal(Controllable * c) override;

	void resizedInternalHeader(Rectangle<int> &r) override;
	void paintOverChildren(Graphics &g) override;

	void newMessage(const Action::ActionEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ActionUI)
};

