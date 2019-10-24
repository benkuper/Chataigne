/*
  ==============================================================================

    StateTransition.cpp
    Created: 28 Oct 2016 8:20:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateTransition.h"
#include "../State/State.h"
#include "../StateManager.h"


StateTransition::StateTransition(State * source, State * dest) :
	Action("transition"),//String(source->niceName << "_" << dest->niceName)),
	sourceState(source),
	destState(dest)
{
	if(sourceState != nullptr)	sourceState->outTransitions.add(this);
	if(destState != nullptr) destState->inTransitions.add(this);

	helpID = "StateTransition";
 }

StateTransition::~StateTransition()
{
	
}

void StateTransition::clearItem()
{
	BaseItem::clearItem();

	if (!sourceState.wasObjectDeleted()) sourceState->outTransitions.removeAllInstancesOf(this);
	if (!destState.wasObjectDeleted()) destState->inTransitions.removeAllInstancesOf(this);
}

var StateTransition::getJSONData()
{
	var data = Action::getJSONData();
	data.getDynamicObject()->setProperty("sourceState", sourceState->shortName);
	data.getDynamicObject()->setProperty("destState", destState->shortName);
	return data;
}

void StateTransition::loadJSONDataInternal(var data)
{
	Action::loadJSONDataInternal(data);
	if (sourceState != nullptr) sourceState->outTransitions.removeAllInstancesOf(this);
	if (destState != nullptr) destState->inTransitions.removeAllInstancesOf(this);
	sourceState = StateManager::getInstance()->getItemWithName(data.getProperty("sourceState", ""));
	destState = StateManager::getInstance()->getItemWithName(data.getProperty("destState", ""));
	if (sourceState != nullptr) sourceState->outTransitions.add(this);
	if (destState != nullptr) destState->inTransitions.add(this);
}

void StateTransition::onContainerTriggerTriggered(Trigger * t)
{
	if (t == triggerOn)
	{
		if (sourceState->active->boolValue())
		{
			Action::onContainerTriggerTriggered(t);
			sourceState->active->setValue(false); //first deactivate this one just in case the dest state instantly reactivates this one...
			destState->active->setValue(true);
		}
	}
}
