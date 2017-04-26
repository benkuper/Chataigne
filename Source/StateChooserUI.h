/*
  ==============================================================================

    StateChooserUI.h
    Created: 20 Feb 2017 2:39:44pm
    Author:  Ben

  ==============================================================================
*/

#ifndef STATECHOOSERUI_H_INCLUDED
#define STATECHOOSERUI_H_INCLUDED

#include "StateManager.h"
#include "TargetParameterUI.h"

class StateChooserUI :
	public TargetParameterUI
{
public:
	enum TargetType { STATE, ACTION, MAPPING };

	StateChooserUI(TargetParameter * p, TargetType type);
	virtual ~StateChooserUI();

	TargetType targetType;

	void updateLabel() override;
	void showPopupAndGetTarget() override;//can be overriden to get specific PopupMenu


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateChooserUI)
};



#endif  // STATECHOOSERUI_H_INCLUDED
