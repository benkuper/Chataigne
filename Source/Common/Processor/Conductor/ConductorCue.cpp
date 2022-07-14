/*
  ==============================================================================

	ConductorCue.cpp
	Created: 5 Oct 2021 9:40:49pm
	Author:  bkupe

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "ConductorCue.h"

ConductorCue::ConductorCue(var params, Multiplex* multiplex) :
	Action("Cue", params, multiplex, true, false),
	isCurrent(false),
	isNext(false),
	index(-1),
	seqCC("Sequence Linking"),
	conductorCueNotifier(5)

{
	linkedSequence = seqCC.addTargetParameter("Linked Sequence", "You can choose a sequence to link for automatic play/stop on cue", ChataigneSequenceManager::getInstance());
	linkedSequence->targetType = TargetParameter::CONTAINER;
	linkedSequence->customGetTargetContainerFunc = ChataigneSequenceManager::getInstance()->showMenuAndGetSequenceStatic;

	autoStart = seqCC.addBoolParameter("Auto Start", "If checked, this will play the sequence on cue", true);
	forceStartFrom0 = seqCC.addBoolParameter("Force Play from start", "If checked, this will force playing the sequence from start", true);
	autoStop = seqCC.addBoolParameter("Auto Stop", "If checked, this will stop the sequence when this cue is not current anymore", true);
	addChildControllableContainer(&seqCC);

	cdm->setHasActivationDefinitions(false, false);
	
	setHasCustomColor(true);
	itemColor->setDefaultValue(Colours::mediumpurple.darker(.2f).withSaturation(.2f));
}

ConductorCue::~ConductorCue()
{
}

void ConductorCue::setIsCurrent(bool value)
{
	if (isCurrent == value) return;
	isCurrent = value;

	if (Sequence* s = dynamic_cast<Sequence*>(linkedSequence->targetContainer.get()))
	{
		if (isCurrent)
		{
			if (autoStart->boolValue())
			{
				if (forceStartFrom0->boolValue()) s->setCurrentTime(0);
				s->playTrigger->trigger();
			}
		}
		else
		{
			if (autoStop->boolValue()) s->stopTrigger->trigger();
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
