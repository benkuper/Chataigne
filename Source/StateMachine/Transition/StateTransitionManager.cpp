/*
  ==============================================================================

	StateTransitionManager.cpp
	Created: 28 Oct 2016 8:20:59pm
	Author:  bkupe

  ==============================================================================
*/

#include "StateMachine/StateMachineIncludes.h"
#include "StateTransitionManager.h"

StateTransitionManager::StateTransitionManager(StateManager* _sm) :
	BaseManager("Transitions"),
	sm(_sm)
{

}

StateTransitionManager::~StateTransitionManager()
{
}

StateTransition* StateTransitionManager::addItemFromData(var data, bool addToUndo)
{
	StateTransition* t = createItemFromData(data);
	if (t == nullptr) return nullptr;
	return addItem(t, data, addToUndo);
}

Array<StateTransition*> StateTransitionManager::addItemsFromData(var data, bool addToUndo)
{
	Array<StateTransition*> itemsToAdd;
	for (int i = 0; i < data.size(); i++)
	{
		StateTransition* t = createItemFromData(data[i]);
		if (t != nullptr) itemsToAdd.add(t);
	}

	return addItems(itemsToAdd, data, addToUndo);
}

StateTransition* StateTransitionManager::createItemFromData(var data)
{
	State* sourceState = sm->getItemWithName(data.getProperty("sourceState", ""));
	State* destState = sm->getItemWithName(data.getProperty("destState", ""));
	if (sourceState == nullptr || destState == nullptr) return nullptr;;
	return createItem(sourceState, destState);
}

StateTransition* StateTransitionManager::createItem(State* source, State* dest)
{
	if (getItemForSourceAndDest(source, dest) != nullptr) return nullptr;
	return new StateTransition(source, dest);
}

StateTransition* StateTransitionManager::createItem()
{
	if (isCurrentlyLoadingData) return new StateTransition(); //allow creation of empty items during loading
	
	return nullptr;
}

Array<State*> StateTransitionManager::getAllStatesLinkedTo(State* state)
{
	Array<State*> result;
	for (auto& t : state->inTransitions) result.add(t->sourceState);
	for (auto& t : state->outTransitions) result.add(t->destState);
	return result;
}

Array<UndoableAction*> StateTransitionManager::getRemoveAllLinkedTransitionsAction(State* linkedState)
{
	Array<StateTransition*> transitionsToRemove;
	transitionsToRemove.addArray(linkedState->inTransitions);
	transitionsToRemove.addArray(linkedState->outTransitions);
	return getRemoveItemsUndoableAction(transitionsToRemove);
}

StateTransition* StateTransitionManager::getItemForSourceAndDest(State* source, State* dest)
{
	for (auto& st : items)
	{
		if (st->sourceState == source && st->destState == dest) return st;
	}
	return nullptr;
}
