/*
  ==============================================================================

    SequenceCommand.cpp
    Created: 20 Feb 2017 2:12:09pm
    Author:  Ben

  ==============================================================================
*/

#include "../SequenceModule.h"
#include "SequenceCommand.h"
#include "TimeMachine/Sequence/Sequence.h"
#include "TimeMachine/Sequence/Layer/SequenceLayer.h"
#include "TimeMachine/SequenceManager.h"
#include "TimeMachine/Sequence/Cue/TimeCue.h"

SequenceCommand::SequenceCommand(SequenceModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	sequenceModule(_module)
{
	target = addTargetParameter("Target", "Target for the command");
	target->targetType = TargetParameter::CONTAINER;

	actionType = (ActionType)(int)params.getProperty("type", PLAY_SEQUENCE);

	switch (actionType)
	{
	case PLAY_SEQUENCE:
	case TOGGLE_SEQUENCE:
		playFromStart = addBoolParameter("Play From Start", "If enabled, when the command is triggered, will position the time at 0 before playing", false);
	case STOP_SEQUENCE:
	case PAUSE_SEQUENCE:
		target->showParentNameInEditor = false;
		target->customGetTargetContainerFunc = &SequenceManager::showMenuAndGetSequence;
		break;

	case DISABLE_LAYER:
	case ENABLE_LAYER:
	case TOGGLE_LAYER:
		target->customGetTargetContainerFunc = &SequenceManager::showmMenuAndGetLayer;
		break;

	case SET_TIME:
		target->customGetTargetContainerFunc = &SequenceManager::showMenuAndGetSequence;

		value = addFloatParameter("Time", "Target time to set", 0, 0, 3600);
		value->defaultUI = FloatParameter::TIME;
		setTargetMappingParameterAt(value, 0);
		break;

	case GOTO_CUE:
		target->customGetTargetContainerFunc = &SequenceManager::showMenuAndGetCue;
		break;

	default:
		break;
	}
}

SequenceCommand::~SequenceCommand()
{
}

void SequenceCommand::trigger()
{
	BaseCommand::trigger();

	if (target->targetContainer == nullptr) return;
	if (target->targetContainer.wasObjectDeleted()) return;

	switch (actionType)
	{
	case PLAY_SEQUENCE:
		if (playFromStart->boolValue()) ((Sequence *)target->targetContainer.get())->stopTrigger->trigger();
		((Sequence *)target->targetContainer.get())->playTrigger->trigger();
		break;

	case PAUSE_SEQUENCE:
		((Sequence *)target->targetContainer.get())->pauseTrigger->trigger();
		break;

	case STOP_SEQUENCE:
		((Sequence *)target->targetContainer.get())->stopTrigger->trigger();
		break;

	case TOGGLE_SEQUENCE:
		if (playFromStart->boolValue() && !((Sequence *)target->targetContainer.get())->isPlaying->boolValue()) ((Sequence *)target->targetContainer.get())->currentTime->setValue(0);
		((Sequence *)target->targetContainer.get())->togglePlayTrigger->trigger();
		break;
            
    case ENABLE_LAYER:
        ((SequenceLayer *)target->targetContainer.get())->enabled->setValue(true);
        break;
            
    case DISABLE_LAYER:
        ((SequenceLayer *)target->targetContainer.get())->enabled->setValue(false);
        break;

	case TOGGLE_LAYER:
		((SequenceLayer *)target->targetContainer.get())->enabled->setValue(!((SequenceLayer *)target->targetContainer.get())->enabled->boolValue());
		break;

	case SET_TIME:
		((Sequence *)target->targetContainer.get())->currentTime->setValue(value->floatValue());
		break;

	case GOTO_CUE:
		TimeCue * cue = dynamic_cast<TimeCue *>(target->targetContainer.get());
		if (cue != nullptr)
		{
			Sequence * s = cue->getSequence();
			if (s != nullptr) s->setCurrentTime(cue->time->floatValue());
		}
		break;
	}
}

void SequenceCommand::loadJSONDataInternal(var data)
{
	if (Engine::mainEngine->isLoadingFile)
	{
		Engine::mainEngine->addEngineListener(this);
		dataToLoad = data;
	}
	else BaseCommand::loadJSONDataInternal(data);
}

void SequenceCommand::endLoadFile()
{
	//reset data we want to reload
	target->setValue("",true);

	loadJSONData(dataToLoad);
	dataToLoad = var();

	Engine::mainEngine->removeEngineListener(this);

}


BaseCommand * SequenceCommand::create(ControllableContainer * module, CommandContext context, var params) {
	return new SequenceCommand((SequenceModule *)module, context, params);
}

/*
InspectableEditor * SequenceCommand::getEditor(bool isRoot)
{
	return new SequenceCommandEditor(this, isRoot);
}
*/
