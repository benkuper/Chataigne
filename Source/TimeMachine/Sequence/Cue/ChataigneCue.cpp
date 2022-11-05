/*
  ==============================================================================

	ChataigneCue.cpp
	Created: 5 Nov 2022 6:47:36pm
	Author:  bkupe

  ==============================================================================
*/

#include "TimeMachine/TimeMachineIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"

ChataigneCue::ChataigneCue(float time, TimeCueManager* m) :
	TimeCue(time, m)
{
	saveAndLoadRecursiveData = true;
}

ChataigneCue::~ChataigneCue()
{

}

bool ChataigneCue::isCurrentlyActive()
{
	if (!TimeCue::isCurrentlyActive()) return false;
	if (!cdm->getIsValid()) return false;
	return true;
}


void ChataigneCue::onContainerParameterChangedInternal(Parameter* p)
{
	TimeCue::onContainerParameterChangedInternal(p);

	if (p == cueAction)
	{
		CueAction a = cueAction->getValueDataAsEnum<CueAction>();
		if (a == PAUSE || a == LOOP_JUMP)
		{
			if (cdm == nullptr)
			{
				cdm.reset(new ConditionManager(nullptr));
				addChildControllableContainer(cdm.get());
				cdm->addConditionManagerListener(this);
				if (ghostCDMData.isObject()) cdm->loadJSONData(ghostCDMData);
			}
		}
		else
		{
			if (cdm != nullptr)
			{
				ghostCDMData = cdm->getJSONData();
				cdm->removeConditionManagerListener(this);

				removeChildControllableContainer(cdm.get());
				cdm.reset();
			}
		}
	}
}

void ChataigneCue::conditionManagerValidationChanged(ConditionManager*, int multiplexIndex, bool dispatchOnChangeOnly)
{
	if (!enabled->boolValue()) return;

	CueAction a = cueAction->getValueDataAsEnum<CueAction>();
	bool valid = cdm->getIsValid();

	if (a == PAUSE && !valid)
	{
		if (Sequence* s = getSequence())
		{
			if (!s->isPlaying->boolValue() && s->currentTime->floatValue() == time->floatValue()) s->playTrigger->trigger();
		}
	}

}
