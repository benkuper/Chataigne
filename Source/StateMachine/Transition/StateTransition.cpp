/*
  ==============================================================================

    StateTransition.cpp
    Created: 28 Oct 2016 8:20:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateMachine/StateMachineIncludes.h"

StateTransition::StateTransition(State * source, State * dest) :
	Action("transition"),//String(source->niceName << "_" << dest->niceName)),
	sourceState(source),
	destState(dest)
{
	if(sourceState != nullptr)	sourceState->outTransitions.add(this);
	if(destState != nullptr) destState->inTransitions.add(this);

	cdm->setHasActivationDefinitions(true, false);

	helpID = "StateTransition";
 }

StateTransition::~StateTransition()
{
	
}

void StateTransition::clearItem()
{
	BaseItem::clearItem();

	if (sourceState != nullptr && !sourceState.wasObjectDeleted()) sourceState->outTransitions.removeAllInstancesOf(this);
	if (destState != nullptr && !destState.wasObjectDeleted()) destState->inTransitions.removeAllInstancesOf(this);
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

void StateTransition::triggerConsequences(bool triggerTrue, int iterationIndex)
{
	if (forceChecking) return;
	if (triggerTrue && sourceState->active->boolValue())
	{
		Action::triggerConsequences(triggerTrue, iterationIndex);

		sourceState->active->setValue(false); //first deactivate this one just in case the dest state instantly reactivates this one...
		destState->active->setValue(true);
	}
}
