/*
  ==============================================================================

    StateTransition.cpp
    Created: 28 Oct 2016 8:20:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateTransition.h"

StateTransition::StateTransition(State * source, State * dest) :
	Action("transition"),
	sourceState(source),
	destState(dest)
{

}

StateTransition::~StateTransition()
{
}

void StateTransition::onContainerTriggerTriggered(Trigger * t)
{
	Action::onContainerTriggerTriggered(t);
	if (t == trigger)
	{
		destState->active->setValue(true);
	}
}
