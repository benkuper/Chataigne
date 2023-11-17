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
	target(nullptr),
	val(nullptr)
{
	actionType = (ActionType)(int)params.getProperty("type", SET_STATE_ACTIVATION);

	if (actionType != CANCEL_ALL_DELAYED_CONSEQUENCES)
	{
		target = addTargetParameter("Target", "Target for the command");
		target->targetType = TargetParameter::CONTAINER;
	}

	switch (actionType)
	{
	case SET_STATE_ACTIVATION:
	case TOGGLE_STATE:
	case CANCEL_STATE_DELAYED_CONSEQUENCES:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetState;
		target->defaultParentLabelLevel = 0;
		break;

	case TRIGGER_ACTION:
	case SET_ACTION_ENABLED:
	case TOGGLE_ACTION:
	case SET_SEQUENTIAL_CONDITION_INDEX:
	case CANCEL_DELAYED_CONSEQUENCE:
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

	case CONDUCTOR_NEXT_TRIGGER:
	case CONDUCTOR_SET_CUE_INDEX:
	case CONDUCTOR_TRIGGER_INDEX:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetConductor;
		target->defaultParentLabelLevel = 1;
		break;

	default:
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
	else if (actionType == CONDUCTOR_SET_CUE_INDEX || actionType == CONDUCTOR_TRIGGER_INDEX)
	{
		val = addIntParameter("Value", "The index of the next cue that will be triggered by the conductor.", 1, 1);
		linkParamToMappingIndex(val, 0);
	}
}

StateCommand::~StateCommand()
{
}

void StateCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	if (actionType != CANCEL_ALL_DELAYED_CONSEQUENCES)
	{
		if (getLinkedTargetContainerAs<ControllableContainer>(target, multiplexIndex) == nullptr) return;
	}

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
		if (Action* a = getLinkedTargetContainerAs<Action>(target, multiplexIndex)) if (a->cdm != nullptr) a->cdm->setSequentialConditionIndices(val->intValue());
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

	case CANCEL_DELAYED_CONSEQUENCE:
		if (Action* a = getLinkedTargetContainerAs<Action>(target, multiplexIndex))
		{
			if (a->csmOn != nullptr) a->csmOn->cancelDelayedConsequences();
			if (a->csmOff != nullptr) a->csmOff->cancelDelayedConsequences();
		}
		break;

	case CANCEL_STATE_DELAYED_CONSEQUENCES:
		if (State* s = getLinkedTargetContainerAs<State>(target, multiplexIndex))
		{
			Array<Action*> actions = s->pm->getAllActions();
			for (auto& a : actions)
			{
				if (a->csmOn != nullptr) a->csmOn->cancelDelayedConsequences();
				if (a->csmOff != nullptr) a->csmOff->cancelDelayedConsequences();
			}
		}
		break;

	case CANCEL_ALL_DELAYED_CONSEQUENCES:
		for (auto& s : StateManager::getInstance()->items)
		{
			Array<Action*> actions = s->pm->getAllActions();
			for (auto& a : actions)
			{
				if (a->csmOn != nullptr) a->csmOn->cancelDelayedConsequences();
				if (a->csmOff != nullptr) a->csmOff->cancelDelayedConsequences();
			}
		}
		break;

	case CONDUCTOR_NEXT_TRIGGER:
		if (Conductor* c = getLinkedTargetContainerAs<Conductor>(target, multiplexIndex)) c->triggerOn->trigger();
		break;

	case CONDUCTOR_SET_CUE_INDEX:
	case CONDUCTOR_TRIGGER_INDEX:
		if (Conductor* c = getLinkedTargetContainerAs<Conductor>(target, multiplexIndex))
		{
			c->nextCueIndex->setValue(val->intValue());
			if (actionType == CONDUCTOR_TRIGGER_INDEX) c->triggerOn->trigger();
		}
		break;
	}
}
