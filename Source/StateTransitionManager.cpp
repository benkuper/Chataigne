/*
  ==============================================================================

    StateTransitionManager.cpp
    Created: 28 Oct 2016 8:20:59pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateTransitionManager.h"

StateTransitionManager::StateTransitionManager() :
	BaseManager("Transitions")
{

}

StateTransitionManager::~StateTransitionManager()
{
}

void StateTransitionManager::addItem(State * source, State * dest, var data)
{
	if (getItemForSourceAndDest(source, dest) != nullptr) return;
	BaseManager::addItem(new StateTransition(source, dest), data);
}

StateTransition * StateTransitionManager::getItemForSourceAndDest(State * source, State * dest)
{
	for (auto &st : items)
	{
		if (st->sourceState == source && st->destState == dest) return st;
	}
	return nullptr;
}
