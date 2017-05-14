/*
  ==============================================================================

    StateTransition.cpp
    Created: 28 Oct 2016 8:20:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateTransition.h"
#include "State.h"

StateTransition::StateTransition(State * source, State * dest) :
	Action("transition"),//String(source->niceName << "_" << dest->niceName)),
	sourceState(source),
	destState(dest)
{
	sourceState->outTransitions.add(this);
	destState->inTransitions.add(this);

 }

StateTransition::~StateTransition()
{
	if(!sourceState.wasObjectDeleted()) sourceState->outTransitions.removeAllInstancesOf(this);
	if(!destState.wasObjectDeleted()) destState->inTransitions.removeAllInstancesOf(this);
}

var StateTransition::getJSONData()
{
	var data = Action::getJSONData();
	data.getDynamicObject()->setProperty("sourceState", sourceState->shortName);
	data.getDynamicObject()->setProperty("destState", destState->shortName);
	return data;
}

void StateTransition::onContainerTriggerTriggered(Trigger * t)
{
	Action::onContainerTriggerTriggered(t);
	if (t == trigger)
	{
		destState->active->setValue(true);
		sourceState->active->setValue(false);
	}
}
