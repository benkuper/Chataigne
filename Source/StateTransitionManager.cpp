/*
  ==============================================================================

    StateTransitionManager.cpp
    Created: 28 Oct 2016 8:20:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateTransitionManager.h"
#include "StateManager.h"

StateTransitionManager::StateTransitionManager(StateManager * _sm) :
	BaseManager("Transitions"),
	sm(_sm)
{

}

StateTransitionManager::~StateTransitionManager()
{
}

void StateTransitionManager::addItemFromData(var data)
{
	State * sourceState = sm->getItemWithName(data.getProperty("sourceState", ""));
	State * destState = sm->getItemWithName(data.getProperty("destState", ""));
	if (sourceState == nullptr || destState == nullptr) return;
	addItem(sourceState, destState, data);
}

void StateTransitionManager::addItem(State * source, State * dest, var data)
{
	if (getItemForSourceAndDest(source, dest) != nullptr) return;
	BaseManager::addItem(new StateTransition(source, dest), data);
}

void StateTransitionManager::removeAllLinkedTransitions(State * linkedState)
{
	Array<StateTransition *> transitionsToRemove;
	transitionsToRemove.addArray(linkedState->inTransitions);
	transitionsToRemove.addArray(linkedState->outTransitions);
	for (auto &st : transitionsToRemove)
	{
		st->remove();
	}
}

StateTransition * StateTransitionManager::getItemForSourceAndDest(State * source, State * dest)
{
	for (auto &st : items)
	{
		if (st->sourceState == source && st->destState == dest) return st;
	}
	return nullptr;
}
