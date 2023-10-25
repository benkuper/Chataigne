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
	public BaseManager<StateTransition>
{
public:
	StateTransitionManager(StateManager * sm);
	~StateTransitionManager();

	StateManager * sm;


	StateTransition* addItemFromData(var data, bool addToUndo = true) override;
	Array<StateTransition*> addItemsFromData(var data, bool addToUndo = true) override;
	StateTransition* createItemFromData(var data);
	StateTransition * createItem(State * source, State * dest);
	StateTransition* createItem() override; //override to avoid 

	Array<State *> getAllStatesLinkedTo(State * state);
	Array<UndoableAction *> getRemoveAllLinkedTransitionsAction(State * linkedState);

	StateTransition * getItemForSourceAndDest(State * source, State * dest);
};