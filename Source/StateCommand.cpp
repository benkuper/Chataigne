/*
  ==============================================================================

    StateCommand.cpp
    Created: 20 Feb 2017 2:12:36pm
    Author:  Ben

  ==============================================================================
*/

#include "StateCommand.h"
#include "TargetParameter.h"
#include "Action.h"
#include "State.h"
#include "StateCommandEditor.h"

StateCommand::StateCommand(StateModule * _module, CommandContext context, var params) :
	BaseCommand(_module,context,params),
	stateModule(_module)
{
	target = addTargetParameter("Target", "Target for the command");
	target->targetType = TargetParameter::CONTAINER;

	actionType = (ActionType)(int)params.getProperty("type", ACTIVATE_STATE);
}

StateCommand::~StateCommand()
{
}

void StateCommand::trigger()
{
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
	if (Engine::getInstance()->isLoadingFile)
	{
		Engine::getInstance()->addEngineListener(this);
		dataToLoad = data;
	}
	else BaseCommand::loadJSONDataInternal(data);
}

void StateCommand::endLoadFile()
{
	Engine::getInstance()->removeEngineListener(this);
	loadJSONData(dataToLoad);
	dataToLoad = var();
}

InspectableEditor * StateCommand::getEditor(bool isRoot)
{
	return new StateCommandEditor(this, isRoot);
}
