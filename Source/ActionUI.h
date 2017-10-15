/*
  ==============================================================================

    ActionUI.h
    Created: 28 Oct 2016 8:05:24pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ACTIONUI_H_INCLUDED
#define ACTIONUI_H_INCLUDED


#include "Action.h"
#include "ProcessorUI.h"

class ActionUI :
	public ProcessorUI
{
public:
	ActionUI(Action *);
	virtual ~ActionUI();

	Action * action;
	void resizedInternalHeader(Rectangle<int> &r) override;

	ScopedPointer<BoolToggleUI> validUI;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ActionUI)
};


#endif  // ACTIONUI_H_INCLUDED
