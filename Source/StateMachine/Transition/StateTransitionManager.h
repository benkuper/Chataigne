/*
  ==============================================================================

    StateTransitionManager.h
    Created: 28 Oct 2016 8:20:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "StateTransition.h"

class StateManager;

class StateTransitionManager :
	public BaseManager<StateTransition>
{
public:
	StateTransitionManager(StateManager * sm);
	~StateTransitionManager();

	StateManager * sm;

	StateTransition * addItemFromData(var data, bool addToUndo = true) override;
	StateTransition * addItem(State * source, State * dest, var data = var(), bool addToUndo = true);

	Array<State *> getAllStatesLinkedTo(State * state);
	Array<UndoableAction *> getRemoveAllLinkedTransitionsAction(State * linkedState);

	StateTransition * getItemForSourceAndDest(State * source, State * dest);
};