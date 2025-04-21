/*
  ==============================================================================

    StateTransitionManager.h
    Created: 28 Oct 2016 8:20:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class StateManager;

class StateTransitionManager :
	public Manager<StateTransition>
{
public:
	StateTransitionManager(StateManager * sm);
	~StateTransitionManager();

	StateManager * sm;


	StateTransition* createItem() override; //override to avoid 
	StateTransition* createItemFromData(var data) override;

	StateTransition * createItem(State * source, State * dest);

	Array<State *> getAllStatesLinkedTo(State * state);
	Array<UndoableAction *> getRemoveAllLinkedTransitionsAction(State * linkedState);

	StateTransition * getItemForSourceAndDest(State * source, State * dest);
};