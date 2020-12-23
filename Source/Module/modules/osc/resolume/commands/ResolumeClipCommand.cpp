/*
  ==============================================================================

    ResolumeClipCommand.cpp
    Created: 3 Nov 2016 12:34:21pm
    Author:  bkupe

  ==============================================================================
*/

#include "ResolumeClipCommand.h"

ResolumeClipCommand::ResolumeClipCommand(ResolumeModule * _module, CommandContext context, var params, IteratorProcessor * iterator) :
	ResolumeBaseCommand(_module,context,params, iterator, true),
	firstClip(nullptr),
	lastClip(nullptr),
	loopClips(nullptr),
	randomClips(nullptr)
{
	connectParam = argumentsContainer.addIntParameter("Connect", "", 1, 0, 1);
	connectParam->setControllableFeedbackOnly(true);
	argumentsContainer.hideInEditor = true;

	needsOnOff = params.getProperty("needsOnOff", false);

	rebuildParameters();
}

ResolumeClipCommand::~ResolumeClipCommand()
{

}

void ResolumeClipCommand::rebuildParametersInternal()
{
	if (params.getProperty("multi", false))
	{
		clipParam->setControllableFeedbackOnly(true);
		firstClip = addIntParameter("First Clip", "First Clip of the MultiClip", 1, 1);
		lastClip = addIntParameter("Last Clip", "Last Clip of the MultiClip", 2, 2);
		loopClips = addBoolParameter("Loop", "Loop when reached last clip", true);
		randomClips = addBoolParameter("Random", "Random between first and last clip", false);
	}
}



void ResolumeClipCommand::onContainerParameterChanged(Parameter * p)
{
	ResolumeBaseCommand::onContainerParameterChanged(p);

	if (p == randomClips)
	{
		loopClips->setEnabled(!randomClips->boolValue());
	} else if (p == firstClip || p == lastClip)
	{
		if (p == firstClip) lastClip->setRange(firstClip->intValue()+1, INT32_MAX);
		else if (p == lastClip) firstClip->setRange(1, lastClip->intValue());
		if (firstClip->intValue() < lastClip->intValue())
		{
			clipParam->setValue(jlimit(firstClip->intValue(), lastClip->intValue(), clipParam->intValue()));
		} else if (firstClip->intValue() == lastClip->intValue())
		{
			clipParam->setValue(firstClip->intValue());
		}
	}
}

void ResolumeClipCommand::triggerInternal(int iterationIndex)
{
	ResolumeBaseCommand::triggerInternal(int iterationIndex);

	if (needsOnOff)
	{
		connectParam->setValue(0);
		ResolumeBaseCommand::triggerInternal(int iterationIndex);
		connectParam->setValue(1);
	}

	if (firstClip != nullptr)
	{
		int targetClip = randomClips->boolValue() ? clipRand.nextInt(Range<int>(firstClip->intValue(),lastClip->intValue()+1)) : clipParam->intValue() + 1;
		if (targetClip > lastClip->intValue())
		{
			targetClip = loopClips->boolValue() ? firstClip->intValue() : lastClip->intValue();
		}
		clipParam->setValue(targetClip);
	}


}
