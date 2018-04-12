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

void StateTransition::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	if (sourceState != nullptr) sourceState->outTransitions.removeAllInstancesOf(this);
	if (destState != nullptr) destState->inTransitions.removeAllInstancesOf(this);
	sourceState = StateManager::getInstance()->getItemWithName(data.getProperty("sourceState", ""));
	destState = StateManager::getInstance()->getItemWithName(data.getProperty("destState", ""));
	if (sourceState != nullptr) sourceState->outTransitions.add(this);
	if (destState != nullptr) destState->inTransitions.add(this);
}

void StateTransition::onContainerTriggerTriggered(Trigger * t)
{
	Action::onContainerTriggerTriggered(t);
	
	if (t == triggerOn)
	{
		
		if (sourceState->active->boolValue())
		{
			DBG("Action trigger ! " << niceName << ", " << sourceState->niceName << ", " << destState->niceName);
			destState->active->setValue(true);
			sourceState->active->setValue(false); //just to be sure but should be handled when activating the dest state
		}
	}
}
