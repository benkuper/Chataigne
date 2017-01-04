/*
  ==============================================================================

    ActionManagerUI.h
    Created: 28 Oct 2016 8:05:53pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef ACTIONMANAGERUI_H_INCLUDED
#define ACTIONMANAGERUI_H_INCLUDED


#include "ActionUI.h"
#include "BaseManagerUI.h"
#include "ActionManager.h"

class ActionManagerUI :
	public BaseManagerUI<ActionManager, Action, ActionUI>
{
public:

	ActionManagerUI(ActionManager *_manager);
	~ActionManagerUI();

	void addItemFromMenu(bool isFromAddButton, Point<int> mouseDownPos) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ActionManagerUI)
};




#endif  // ACTIONMANAGERUI_H_INCLUDED
