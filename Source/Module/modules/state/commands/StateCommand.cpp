/*
  ==============================================================================

    StateCommand.cpp
    Created: 20 Feb 2017 2:12:36pm
    Author:  Ben

  ==============================================================================
*/

#include "StateCommand.h"
#include "StateMachine/StateManager.h"
#include "Common/Processor/Action/Condition/conditions/StandardCondition/StandardCondition.h"
#include "Common/Processor/Mapping/Mapping.h"

StateCommand::StateCommand(StateModule * _module, CommandContext context, var params, Multiplex * multiplex) :
	BaseCommand(_module,context,params),
	stateModule(_module),
	enableVal(nullptr)
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
		enableVal = addBoolParameter("Value", "The activation / enable state to set this element to.", true);
		linkParamToMappingIndex(enableVal, 0);
	}
}

StateCommand::~StateCommand()
{
}

void StateCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	if (target->targetContainer == nullptr) return; 
	if (target->targetContainer.wasObjectDeleted()) return;
	
	switch (actionType)
	{
	case SET_STATE_ACTIVATION:
		((State *)target->targetContainer.get())->active->setValue(enableVal->boolValue());
		break;

	case TOGGLE_STATE:
		((State *)target->targetContainer.get())->active->setValue(!((State *)target->targetContainer.get())->active->boolValue());
		break;

	case TRIGGER_ACTION:
		if(((Action*)target->targetContainer.get())->triggerOn != nullptr) ((Action*)target->targetContainer.get())->triggerOn->trigger();
		break;

	case SET_ACTION_ENABLED:
		((Action *)target->targetContainer.get())->enabled->setValue(enableVal->boolValue());
		break;

	case TOGGLE_ACTION:
		((Action *)target->targetContainer.get())->enabled->setValue(!((Action *)target->targetContainer.get())->enabled->boolValue());
		break;

	case SET_TOGGLE_STATE:
	{
		(((StandardCondition*)target->targetContainer.get())->forceToggleState(enableVal->boolValue()));
	}
	break;

	case SET_MAPPING_ENABLED:
		((Mapping *)target->targetContainer.get())->enabled->setValue(enableVal->boolValue());
		break;

	case TOGGLE_MAPPING:
		((Mapping *)target->targetContainer.get())->enabled->setValue(!((Mapping *)target->targetContainer.get())->enabled->boolValue());
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
