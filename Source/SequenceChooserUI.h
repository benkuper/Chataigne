/*
  ==============================================================================

    SequenceChooserUI.h
    Created: 20 Feb 2017 4:16:00pm
    Author:  Ben

  ==============================================================================
*/

#ifndef SEQUENCECHOOSERUI_H_INCLUDED
#define SEQUENCECHOOSERUI_H_INCLUDED

#include "StateManager.h"
#include "TargetParameterUI.h"

class SequenceChooserUI :
	public TargetParameterUI
{
public:
	enum TargetType { SEQUENCE, LAYER };

	SequenceChooserUI(TargetParameter * p, TargetType type);
	virtual ~SequenceChooserUI();

	TargetType targetType;

	void updateLabel() override;
	void showPopupAndGetTarget();//can be overriden to get specific PopupMenu


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceChooserUI)
};




#endif  // SEQUENCECHOOSERUI_H_INCLUDED
