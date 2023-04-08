/*
  ==============================================================================

	ConductorCue.cpp
	Created: 5 Oct 2021 9:40:49pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"

ConductorCue::ConductorCue(var params, Multiplex* multiplex) :
	Action("Cue", params, multiplex, true, false),
	isCurrent(false),
	isNext(false),
	index(-1),
	seqCC("Sequence Linking"),
	linkedSequence(nullptr),
	conductorCueNotifier(5)

{
	linkedSequenceParam = seqCC.addTargetParameter("Linked Sequence", "You can choose a sequence to link for automatic play/stop on cue", ChataigneSequenceManager::getInstance());
	linkedSequenceParam->targetType = TargetParameter::CONTAINER;
	linkedSequenceParam->customGetTargetContainerFunc = ChataigneSequenceManager::getInstance()->showMenuAndGetSequenceStatic;

	autoStart = seqCC.addBoolParameter("Auto Start", "If checked, this will play the sequence on cue", true);
	forceStartFrom0 = seqCC.addBoolParameter("Force Play from start", "If checked, this will force playing the sequence from start", true);
	autoStop = seqCC.addBoolParameter("Auto Stop", "If checked, this will stop the sequence when this cue is not current anymore", true);
	autoNext = seqCC.addBoolParameter("Auto Next On Finish", "If checked, this will trigger the next cue when the linked sequence is finished", false);

	addChildControllableContainer(&seqCC);

	cdm->setHasActivationDefinitions(false, false);

	setHasCustomColor(true);
	itemColor->setDefaultValue(Colours::mediumpurple.darker(.2f).withSaturation(.2f));
}

ConductorCue::~ConductorCue()
{
}

void ConductorCue::clearItem()
{
	setLinkedSequence(nullptr);
	Action::clearItem();
}

void ConductorCue::setLinkedSequence(Sequence* s)
{
	if (linkedSequence != nullptr)
	{
		linkedSequence->removeSequenceListener(this);
	}

	linkedSequence = s;

	if (linkedSequence != nullptr)
	{
		linkedSequence->addSequenceListener(this);
	}
}

void ConductorCue::setIsCurrent(bool value)
{
	if (isCurrent == value) return;
	isCurrent = value;

	if (linkedSequence != nullptr)
	{
		if (isCurrent)
		{
			if (autoStart->boolValue())
			{
				if (forceStartFrom0->boolValue()) linkedSequence->setCurrentTime(0);
				linkedSequence->playTrigger->trigger();
			}
		}
		else
		{
			if (autoStop->boolValue()) linkedSequence->stopTrigger->trigger();
		}
	}
	conductorCueNotifier.addMessage(new ConductorCueEvent(ConductorCueEvent::CURRENT_CHANGED, this));
}

void ConductorCue::setIsNext(bool value)
{
	if (isNext == value) return;
	isNext = value;
	conductorCueNotifier.addMessage(new ConductorCueEvent(ConductorCueEvent::NEXT_CHANGED, this));
}

void ConductorCue::setIndex(int value)
{
	if (index == value) return;
	index = value;
	conductorCueNotifier.addMessage(new ConductorCueEvent(ConductorCueEvent::INDEX_CHANGED, this));
}

void ConductorCue::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Action::onControllableFeedbackUpdateInternal(cc, c);
	if (c == linkedSequenceParam)
	{
		setLinkedSequence(dynamic_cast<Sequence*>(linkedSequenceParam->targetContainer.get()));
	}
}

void ConductorCue::sequenceFinished(Sequence* s)
{
	if (autoNext->boolValue())
	{
		if (Conductor* c = dynamic_cast<Conductor*>(parentContainer->parentContainer.get()))
		{
			c->triggerOn->trigger();
		}
	}
}


ProcessorUI* ConductorCue::getUI()
{
	return new ConductorCueUI(this);
}

var ConductorCue::getJSONData()
{
	var data = Action::getJSONData();
	data.getDynamicObject()->setProperty(seqCC.shortName, seqCC.getJSONData());
	return data;
}

void ConductorCue::loadJSONDataItemInternal(var data)
{
	Action::loadJSONDataItemInternal(data);
	seqCC.loadJSONData(data.getProperty(seqCC.shortName, var()));
}
