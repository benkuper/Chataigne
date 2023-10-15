/*
  ==============================================================================

	SequenceCommand.cpp
	Created: 20 Feb 2017 2:12:09pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

SequenceCommand::SequenceCommand(SequenceModule* _module, CommandContext context, var params, Multiplex* multiplex) :
	BaseCommand(_module, context, params, multiplex),
	sequenceModule(_module),
	target(nullptr)
{
	actionType = (ActionType)(int)params.getProperty("type", PLAY_SEQUENCE);

	if (actionType != STOP_ALL_SEQUENCES && actionType != PLAY_MULTI_SEQUENCES && actionType != PLAY_SEQUENCE_AT && actionType != SET_EDITING_SEQUENCE_AT)
	{
		target = addTargetParameter("Target", "Target for the command");
		target->targetType = TargetParameter::CONTAINER;
	}

	if (actionType == PLAY_MULTI_SEQUENCES)
	{
		paramsCanBeLinked = false;

		resetIncrement = addTrigger("Reset", "This will reset the increment to 0");
		minIndex = addIntParameter("Min Index", "This is the index of the first sequence that will play", 0, 0);
		maxIndex = addIntParameter("Max Index", "This is the index of the last sequence that will play", 10, 0);
		currentSequenceIndex = addIntParameter("Sequence Index", "This is the index of the sequence that will play on trigger", 0, 0);
		playFromStart = addBoolParameter("Play From Start", "If enabled, when the command is triggered, will position the time at 0 before playing", false);
		loopMulti = addBoolParameter("Loop", "If enabled, this will go back to the min index", false);
	}

	if (actionType == PLAY_SEQUENCE_AT || actionType == SET_EDITING_SEQUENCE_AT)
	{
		currentSequenceIndex = addIntParameter("Sequence Index", "This is the index of the sequence that will play on trigger", 0, 0);
	}

	switch (actionType)
	{
	case PLAY_SEQUENCE_AT:
		playFromStart = addBoolParameter("Play From Start", "If enabled, when the command is triggered, will position the time at 0 before playing", false);
		break;

	case PLAY_SEQUENCE:
	case TOGGLE_SEQUENCE:
		playFromStart = addBoolParameter("Play From Start", "If enabled, when the command is triggered, will position the time at 0 before playing", false);
	case STOP_SEQUENCE:
	case PAUSE_SEQUENCE:
	case SET_EDITING_SEQUENCE:
		target->showParentNameInEditor = false;
		target->customGetTargetContainerFunc = &ChataigneSequenceManager::showMenuAndGetSequenceStatic;
		break;


	case DISABLE_LAYER:
	case ENABLE_LAYER:
	case TOGGLE_LAYER:
		target->customGetTargetContainerFunc = &ChataigneSequenceManager::showMenuAndGetLayerStatic;
		break;

	case SET_TIME:
	case MOVE_TIME:
		target->customGetTargetContainerFunc = &ChataigneSequenceManager::showMenuAndGetSequenceStatic;

		value = addFloatParameter("Time", "Target time to set", 0);
		((FloatParameter*)value)->defaultUI = FloatParameter::TIME;
		linkParamToMappingIndex(value, 0);

		playFromStart = addBoolParameter("Play", "If enabled, will force playing the sequence after setting the time", false);
		break;

	case GOTO_CUE:
		target->customGetTargetContainerFunc = &ChataigneSequenceManager::showMenuAndGetCueStatic;
		playFromStart = addBoolParameter("Play", "If enabled, will force playing the sequence after setting the time to the cue", false);
		break;

	case GOTO_PREV_CUE:
	case GOTO_NEXT_CUE:
		target->customGetTargetContainerFunc = &ChataigneSequenceManager::showMenuAndGetSequenceStatic;
		playFromStart = addBoolParameter("Play", "If enabled, will force playing the sequence after setting the time to the cue", false);
		break;

	case SET_TRIGGER_ENABLED:
		target->customGetTargetContainerFunc = &ChataigneSequenceManager::showMenuAndGetTriggerStatic;
		value = addBoolParameter("Value", "The enable state to set this trigger", true);
		break;

	default:
		break;
	}
}

SequenceCommand::~SequenceCommand()
{
}

void SequenceCommand::triggerInternal(int multiplexIndex)
{
	BaseCommand::triggerInternal(multiplexIndex);

	if (actionType != STOP_ALL_SEQUENCES && actionType != PLAY_MULTI_SEQUENCES && actionType != PLAY_SEQUENCE_AT)
	{
		if (getLinkedTargetContainerAs<ControllableContainer>(target, multiplexIndex) == nullptr) return;
		//if (target->targetContainer.wasObjectDeleted()) return;
	}

	switch (actionType)
	{

	case PLAY_SEQUENCE:
	{
		if (Sequence* s = getLinkedTargetContainerAs<Sequence>(target, multiplexIndex))
		{
			if (getLinkedValue(playFromStart, multiplexIndex)) s->stopTrigger->trigger();
			s->playTrigger->trigger();
		}
	}
	break;

	case PLAY_SEQUENCE_AT:
	case SET_EDITING_SEQUENCE_AT:
	{
		int index = getLinkedValue(currentSequenceIndex, multiplexIndex);
		if (index >= 0 && index < ChataigneSequenceManager::getInstance()->items.size())
		{
			if (Sequence* s = ChataigneSequenceManager::getInstance()->items[index])
			{
				if (actionType == SET_EDITING_SEQUENCE_AT)
				{
					if (Sequence* s = getLinkedTargetContainerAs<Sequence>(target, multiplexIndex))
					{
						if (ShapeShifterManager* sm = ShapeShifterManager::getInstanceWithoutCreating())
						{
							if (TimeMachineView* se = sm->getContentForType<TimeMachineView>())
							{
								se->setSequence(s);
							}
						}
					}
				}
				else if (actionType == PLAY_SEQUENCE_AT)
				{
					if ((int)getLinkedValue(playFromStart, multiplexIndex)) s->stopTrigger->trigger();
					s->playTrigger->trigger();
				}
			}
		}
	}
	break;

	case PLAY_MULTI_SEQUENCES:
	{
		int numSequences = ChataigneSequenceManager::getInstance()->items.size();
		if (currentSequenceIndex->intValue() < numSequences)
		{
			Sequence* s = ChataigneSequenceManager::getInstance()->items[currentSequenceIndex->intValue()];

			if (getLinkedValue(playFromStart, multiplexIndex)) s->stopTrigger->trigger();
			s->playTrigger->trigger();

			int targetIndex = currentSequenceIndex->intValue() + 1;
			if (targetIndex > maxIndex->intValue() && loopMulti->boolValue()) targetIndex = minIndex->intValue();

			currentSequenceIndex->setValue(targetIndex);
		}
		else
		{
			NLOGWARNING(niceName, "Sequence #" << currentSequenceIndex->intValue() << "doesn't exist");
			if (loopMulti->boolValue()) currentSequenceIndex->setValue(minIndex->intValue());
		}
	}
	break;

	case PAUSE_SEQUENCE:
		if (Sequence* s = getLinkedTargetContainerAs<Sequence>(target, multiplexIndex)) s->pauseTrigger->trigger();
		break;

	case STOP_SEQUENCE:
		if (Sequence* s = getLinkedTargetContainerAs<Sequence>(target, multiplexIndex)) s->stopTrigger->trigger();
		break;

	case STOP_ALL_SEQUENCES:
		ChataigneSequenceManager::getInstance()->stopAllTrigger->trigger();
		break;

	case TOGGLE_SEQUENCE:
		if (Sequence* s = getLinkedTargetContainerAs<Sequence>(target, multiplexIndex))
		{
			if (getLinkedValue(playFromStart, multiplexIndex) && !s->isPlaying->boolValue()) s->setCurrentTime(0);
			s->togglePlayTrigger->trigger();
		}
		break;

	case ENABLE_LAYER:
		if (SequenceLayer* layer = getLinkedTargetContainerAs<SequenceLayer>(target, multiplexIndex)) layer->enabled->setValue(true);
		break;

	case DISABLE_LAYER:
		if (SequenceLayer* layer = getLinkedTargetContainerAs<SequenceLayer>(target, multiplexIndex)) layer->enabled->setValue(false);
		break;

	case TOGGLE_LAYER:
		if (SequenceLayer* layer = getLinkedTargetContainerAs<SequenceLayer>(target, multiplexIndex)) layer->enabled->setValue(!layer->enabled->boolValue());
		break;

	case SET_TIME:
	case MOVE_TIME:
	{
		if (Sequence* s = getLinkedTargetContainerAs<Sequence>(target, multiplexIndex))
		{
			float t = actionType == SET_TIME ? 0 : s->currentTime->floatValue();
			s->setCurrentTime(t + (float)getLinkedValue(value, multiplexIndex), true, true);
			if (getLinkedValue(playFromStart, multiplexIndex)) s->playTrigger->trigger();
		}
	}
	break;

	case GOTO_CUE:
		if (TimeCue* cue = getLinkedTargetContainerAs<TimeCue>(target, multiplexIndex))
		{
			Sequence* s = cue->getSequence();
			s->setCurrentTime(cue->time->floatValue(), true, true);
			if (getLinkedValue(playFromStart, multiplexIndex)) s->playTrigger->trigger();
		}
		break;

	case GOTO_PREV_CUE:
	case GOTO_NEXT_CUE:
		if (Sequence* s = getLinkedTargetContainerAs<Sequence>(target, multiplexIndex))
		{
			Trigger* t = (actionType == GOTO_PREV_CUE) ? s->prevCue : s->nextCue;
			t->trigger();
			if (getLinkedValue(playFromStart, multiplexIndex)) s->playTrigger->trigger();
		}
		break;

	case SET_TRIGGER_ENABLED:
		if (TimeTrigger* tt = getLinkedTargetContainerAs<TimeTrigger>(target, multiplexIndex))
		{
			tt->enabled->setValue(getLinkedValue(value, multiplexIndex));
		}
		break;

	case SET_EDITING_SEQUENCE:
		if (Sequence* s = getLinkedTargetContainerAs<Sequence>(target, multiplexIndex))
		{
			if (ShapeShifterManager* sm = ShapeShifterManager::getInstanceWithoutCreating())
			{
				if (TimeMachineView* se = sm->getContentForType<TimeMachineView>())
				{
					se->setSequence(s);
				}
			}
		}
	}
}

void SequenceCommand::onContainerTriggerTriggered(Trigger* t)
{
	BaseCommand::onContainerTriggerTriggered(t);
	if (t == resetIncrement) currentSequenceIndex->setValue(minIndex->intValue());
}

void SequenceCommand::onContainerParameterChanged(Parameter* p)
{
	BaseCommand::onContainerParameterChanged(p);
	if (p == minIndex || p == maxIndex)
	{
		currentSequenceIndex->setRange(minIndex->intValue(), maxIndex->intValue());
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
	if (target != nullptr) target->setValue("", true);

	loadJSONData(dataToLoad);
	dataToLoad = var();

	Engine::mainEngine->removeEngineListener(this);

}


BaseCommand* SequenceCommand::create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex) {
	return new SequenceCommand((SequenceModule*)module, context, params, multiplex);
}

/*
InspectableEditor * SequenceCommand::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new SequenceCommandEditor(this, isRoot);
}
*/
