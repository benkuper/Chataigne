/*
  ==============================================================================

    StateManagerUI.h
    Created: 28 Oct 2016 8:19:25pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEMANAGERUI_H_INCLUDED
#define STATEMANAGERUI_H_INCLUDED


#include "StateManager.h"
#include "BaseManagerUI.h"
#include "StateUI.h"

class StateManagerUI :
	public BaseManagerUI<StateManager, State, StateUI>
{
public:
	StateManagerUI(StateManager *_manager);
	~StateManagerUI();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateManagerUI)
};




#endif  // STATEMANAGERUI_H_INCLUDED
