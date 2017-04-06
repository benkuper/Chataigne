/*
  ==============================================================================

	SequenceCueCommandEditor.cpp
	Created: 6 Apr 2017 12:29:26pm
	Author:  Ben

  ==============================================================================
*/

#include "SequenceCueCommandEditor.h"
#include "Sequence.h"

SequenceCueCommandEditor::SequenceCueCommandEditor(SequenceCueCommand * command, bool isRoot) :
	SequenceCommandEditor(command, isRoot),
	cueCommand(command)
{
	cueTargetUI = new SequenceChooserUI(cueCommand->targetCue, SequenceChooserUI::CUE);
	addAndMakeVisible(cueTargetUI);

	cueTargetUI->sequenceForCue = static_cast<Sequence *>(cueCommand->target->targetContainer.get());

	cueCommand->addAsyncContainerListener(this);
	setSize(10, 50);
}

SequenceCueCommandEditor::~SequenceCueCommandEditor()
{
	cueCommand->removeAsyncContainerListener(this);
}

void SequenceCueCommandEditor::resized()
{
	SequenceCommandEditor::resized();
	cueTargetUI->setBounds(chooserUI->getBounds().withY(chooserUI->getBottom() + 10));
}

void SequenceCueCommandEditor::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == ContainerAsyncEvent::ControllableFeedbackUpdate)
	{
		if (e.targetControllable == cueCommand->target)
		{
			cueTargetUI->sequenceForCue = static_cast<Sequence *>(cueCommand->target->targetContainer.get());
		}
	}
}
