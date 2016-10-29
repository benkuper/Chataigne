/*
  ==============================================================================

    StateManager.h
    Created: 28 Oct 2016 8:19:15pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEMANAGER_H_INCLUDED
#define STATEMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "State.h"

class StateManager :
	public BaseManager<State>
{
public:
	juce_DeclareSingleton(StateManager, false)

	StateManager();
	~StateManager();

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateManager)
};





#endif  // STATEMANAGER_H_INCLUDED
