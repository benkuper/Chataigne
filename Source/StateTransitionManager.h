/*
  ==============================================================================

    StateTransitionManager.h
    Created: 28 Oct 2016 8:20:59pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATETRANSITIONMANAGER_H_INCLUDED
#define STATETRANSITIONMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "StateTransition.h"

class StateManager;

class StateTransitionManager :
	public BaseManager<StateTransition>
{
public:
	StateTransitionManager(StateManager * sm);
	~StateTransitionManager();

	StateManager *sm;

	void addItemFromData(var data, bool fromUndoableAction = false) override;

	void addItem(State * source, State * dest, var data = var(), bool fromUndoableAction = false);

	void removeAllLinkedTransitions(State * linkedState);

	StateTransition * getItemForSourceAndDest(State * source, State * dest);
};



#endif  // STATETRANSITIONMANAGER_H_INCLUDED
