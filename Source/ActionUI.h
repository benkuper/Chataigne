/*
  ==============================================================================

    ActionUI.h
    Created: 28 Oct 2016 8:05:24pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ACTIONUI_H_INCLUDED
#define ACTIONUI_H_INCLUDED


#include "BaseItemUI.h"
#include "Action.h"
#include "BoolToggleUI.h"

class ActionUI :
	public BaseItemUI<Action>
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
