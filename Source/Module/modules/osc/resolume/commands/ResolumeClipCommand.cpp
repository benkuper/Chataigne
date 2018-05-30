/*
  ==============================================================================

    ResolumeClipCommand.cpp
    Created: 3 Nov 2016 12:34:21pm
    Author:  bkupe

  ==============================================================================
*/

#include "ResolumeClipCommand.h"

ResolumeClipCommand::ResolumeClipCommand(ResolumeModule * _module, CommandContext context, var params) :
	ResolumeBaseCommand(_module,context,params),
	firstClip(nullptr),
	lastClip(nullptr),
	loopClips(nullptr),
	randomClips(nullptr)
{
	connectParam = argumentsContainer.addIntParameter("Connect", "", 1, 1, 1);
	argumentsContainer.hideInEditor = true;

	address->isEditable = false;


	if (params.getProperty("multi", false))
	{
		clipParam->setControllableFeedbackOnly(true);
		firstClip = addIntParameter("First Clip", "First Clip of the MultiClip", 1, 1,INT32_MAX);
		lastClip = addIntParameter("Last Clip", "Last Clip of the MultiClip", 1, 1, INT32_MAX);
		loopClips = addBoolParameter("Loop", "Loop when reached last clip", true);
		randomClips = addBoolParameter("Random", "Random between first and last clip", false);
	}

	rebuildAddress();
}

ResolumeClipCommand::~ResolumeClipCommand()
{

}

void ResolumeClipCommand::onContainerParameterChanged(Parameter * p)
{
	ResolumeBaseCommand::onContainerParameterChanged(p);

	if (p == randomClips)
	{
		loopClips->setEnabled(!randomClips->boolValue());
	} else if (p == firstClip || p == lastClip)
	{
		if (p == firstClip) lastClip->setRange(firstClip->intValue(), 100);
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

void ResolumeClipCommand::trigger()
{
	ResolumeBaseCommand::trigger();

	if (firstClip != nullptr)
	{
		int targetClip = randomClips->boolValue() ? clipRand.nextInt(Range<int>(firstClip->intValue(),lastClip->intValue())) : clipParam->intValue() + 1;
		if (targetClip > lastClip->intValue())
		{
			targetClip = loopClips->boolValue() ? firstClip->intValue() : lastClip->intValue();
		}
		clipParam->setValue(targetClip);
	}
}
