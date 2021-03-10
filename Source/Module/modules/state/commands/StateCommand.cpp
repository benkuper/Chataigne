/*
  ==============================================================================

	StateCommand.cpp
	Created: 20 Feb 2017 2:12:36pm
	Author:  Ben

  ==============================================================================
*/

#include "StateMachine/StateMachineIncludes.h"

StateCommand::StateCommand(StateModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	stateModule(_module),
	val(nullptr)
{
	target = addTargetParameter("Target", "Target for the command");
	target->targetType = TargetParameter::CONTAINER;
	actionType = (ActionType)(int)params.getProperty("type", SET_STATE_ACTIVATION);

	switch (actionType)
	{
	case SET_STATE_ACTIVATION:
	case TOGGLE_STATE:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetState;
		target->defaultParentLabelLevel = 0;
		break;

	case TRIGGER_ACTION:
	case SET_ACTION_ENABLED:
	case TOGGLE_ACTION:
	case SET_SEQUENTIAL_CONDITION_INDEX:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetAction;
		target->defaultParentLabelLevel = 1;
		break;

	case SET_TOGGLE_STATE:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetToggleCondition;
		target->defaultParentLabelLevel = 2;
		break;

	case SET_MAPPING_ENABLED:
	case TOGGLE_MAPPING:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetMapping;
		target->defaultParentLabelLevel = 1;
		break;
	}

	if (actionType == SET_STATE_ACTIVATION || actionType == SET_ACTION_ENABLED || actionType == SET_TOGGLE_STATE || actionType == SET_MAPPING_ENABLED)
	{
		val = addBoolParameter("Value", "The activation / enable state to set this element to.", true);
		linkParamToMappingIndex(val, 0);
	}
	else if (actionType == SET_SEQUENTIAL_CONDITION_INDEX)
	{
		val = addIntParameter("Value", "The activation / enable state to set this element to.", 0, 0);
		linkParamToMappingIndex(val, 0);
	}
}

StateCommand::~StateCommand()
{
}

void StateCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	if (getLinkedTargetContainerAs<ControllableContainer>(target, multiplexIndex) == nullptr) return;
	//if (target->targetContainer.wasObjectDeleted()) return;

	switch (actionType)
	{
	case SET_STATE_ACTIVATION:
		if (State* s = getLinkedTargetContainerAs<State>(target, multiplexIndex)) s->active->setValue(val->boolValue());
		break;

	case TOGGLE_STATE:
		if (State* s = getLinkedTargetContainerAs<State>(target, multiplexIndex)) s->active->setValue(!s->active->boolValue());
		break;

	case TRIGGER_ACTION:
		if (Action* a = getLinkedTargetContainerAs<Action>(target, multiplexIndex)) if (a->triggerOn != nullptr) a->triggerOn->trigger();
		break;

	case SET_ACTION_ENABLED:
		if (Action* a = getLinkedTargetContainerAs<Action>(target, multiplexIndex)) a->enabled->setValue(val->boolValue());
		break;

	case TOGGLE_ACTION:
		if (Action* a = getLinkedTargetContainerAs<Action>(target, multiplexIndex)) a->enabled->setValue(!a->enabled->boolValue());
		break;

	case SET_SEQUENTIAL_CONDITION_INDEX:
		if (Action* a = getLinkedTargetContainerAs<Action>(target, multiplexIndex)) a->cdm.setSequentialConditionIndices(val->intValue());
		break;

	case SET_TOGGLE_STATE:
		if (StandardCondition* cd = getLinkedTargetContainerAs<StandardCondition>(target, multiplexIndex)) cd->forceToggleState(val->boolValue());
		break;

	case SET_MAPPING_ENABLED:
		if (Mapping* m = getLinkedTargetContainerAs<Mapping>(target, multiplexIndex)) m->enabled->setValue(val->boolValue());
		break;

	case TOGGLE_MAPPING:
		if (Mapping* m = getLinkedTargetContainerAs<Mapping>(target, multiplexIndex)) m->enabled->setValue(!m->enabled->boolValue());
		break;
	}
}

void StateCommand::loadJSONDataInternal(var data)
{
	if (Engine::mainEngine->isLoadingFile)
	{
		Engine::mainEngine->addEngineListener(this);
		dataToLoad = data;
	}
	else BaseCommand::loadJSONDataInternal(data);
}

void StateCommand::endLoadFile()
{
	target->setValue("", true);

	loadJSONData(dataToLoad);
	dataToLoad = var();

	Engine::mainEngine->removeEngineListener(this);
}
