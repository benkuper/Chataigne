/*
  ==============================================================================

    StateManager.cpp
    Created: 28 Oct 2016 8:19:15pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateManager.h"

juce_ImplementSingleton(StateManager)

StateManager::StateManager() :
	BaseManager<State>("States"),
	activeState(nullptr)
{
}

StateManager::~StateManager()
{
}

void StateManager::setActiveState(State * s)
{
	if (s == activeState) return;

	if (!activeState.wasObjectDeleted() && activeState != nullptr)
	{
		activeState->active->setValue(false);
	}

	activeState = s;
}



State * StateManager::addItem(const Point<float>& initialPosition)
{
	State * s = BaseManager::addItem();
	s->viewUIPosition->setPoint(initialPosition);
	return s;
}

void StateManager::addItemInternal(State * s, var data)
{
	s->addStateListener(this);
	if (s->active->boolValue()) setActiveState(s);
	else if (activeState == nullptr) s->active->setValue(true);
}

void StateManager::removeItemInternal(State * s)
{
	s->removeStateListener(this);
}

void StateManager::stateActivationChanged(State * s)
{
	if (s->permanent->boolValue()) return; //don't care of permanent in state logic.
	if (s->active->boolValue()) setActiveState(s);
}
