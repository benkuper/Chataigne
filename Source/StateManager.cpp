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
	BaseManager<State>("States")
{
}

StateManager::~StateManager()
{
}

State * StateManager::addItem(const Point<float>& initialPosition)
{
	State * s = BaseManager<State>::addItem();
	s->viewUIPosition->setPoint(initialPosition);
	return s;
}
