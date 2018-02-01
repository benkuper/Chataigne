/*
  ==============================================================================

    StateCommand.cpp
    Created: 20 Feb 2017 2:12:36pm
    Author:  Ben

  ==============================================================================
*/

#include "StateCommand.h"
#include "StateManager.h"

StateCommand::StateCommand(StateModule * _module, CommandContext context, var params) :
	BaseCommand(_module,context,params),
	stateModule(_module)
{
	target = addTargetParameter("Target", "Target for the command");
	target->targetType = TargetParameter::CONTAINER;
	actionType = (ActionType)(int)params.getProperty("type", ACTIVATE_STATE);

	switch (actionType)
	{
	case ACTIVATE_STATE:
	case DEACTIVATE_STATE:
	case TOGGLE_STATE:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetState;
		break;

	case TRIGGER_ACTION:
	case ENABLE_ACTION:
	case DISABLE_ACTION:
	case TOGGLE_ACTION:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetAction;
		break;

	case ENABLE_MAPPING:
	case DISABLE_MAPPING:
	case TOGGLE_MAPPING:
		target->customGetTargetContainerFunc = &StateManager::showMenuAndGetMapping;
		break;

	}
}

StateCommand::~StateCommand()
{
}

void StateCommand::trigger()
{
	BaseCommand::trigger();

	if (target->targetContainer == nullptr) return; 
	if (target->targetContainer.wasObjectDeleted()) return;
	
	switch (actionType)
	{
	case ACTIVATE_STATE:
		((State *)target->targetContainer.get())->active->setValue(true);
		break;

	case DEACTIVATE_STATE:
		((State *)target->targetContainer.get())->active->setValue(false);
		break;

	case TOGGLE_STATE:
		((State *)target->targetContainer.get())->active->setValue(!((State *)target->targetContainer.get())->active->boolValue());
		break;

	case TRIGGER_ACTION:
		((Action *)target->targetContainer.get())->trigger->trigger();
		break;

	case ENABLE_ACTION:
		((Action *)target->targetContainer.get())->enabled->setValue(true);
		break;

	case DISABLE_ACTION:
		((Action *)target->targetContainer.get())->enabled->setValue(false);
		break;

	case TOGGLE_ACTION:
		((Action *)target->targetContainer.get())->enabled->setValue(!((Action *)target->targetContainer.get())->enabled->boolValue());
		break;

	case ENABLE_MAPPING:
		((Mapping *)target->targetContainer.get())->enabled->setValue(true);
		break;

	case DISABLE_MAPPING:
		((Mapping *)target->targetContainer.get())->enabled->setValue(false);
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