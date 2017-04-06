/*
  ==============================================================================

    SequenceCueCommand.cpp
    Created: 6 Apr 2017 12:29:13pm
    Author:  Ben

  ==============================================================================
*/

#include "SequenceCueCommand.h"
#include "SequenceCueCommandEditor.h"
#include "Sequence.h"
#include "TimeCue.h"

SequenceCueCommand::SequenceCueCommand(SequenceModule * _module, CommandContext context, var params) :
	SequenceCommand(_module,context,params)
{
	targetCue = addTargetParameter("Target Cue", "Cue that this command will target", nullptr);
	targetCue->targetType = TargetParameter::CONTAINER;
}

SequenceCueCommand::~SequenceCueCommand()
{
}

void SequenceCueCommand::trigger()
{
	if (target->targetContainer == nullptr) return;
	if (target->targetContainer.wasObjectDeleted()) return;

	Sequence * sequence = static_cast<Sequence *>(target->targetContainer.get());
	TimeCue * cue = static_cast<TimeCue *>(targetCue->targetContainer.get());
	if (sequence && cue)
	{
		sequence->setCurrentTime(cue->time->floatValue());
	}
}

void SequenceCueCommand::endLoadFile()
{
	targetCue->setValue("",true);
	SequenceCommand::endLoadFile();
}

BaseCommand * SequenceCueCommand::create(ControllableContainer * module, CommandContext context, var params)
  {
	return new SequenceCueCommand((SequenceModule *)module, context, params);
  }

  InspectableEditor * SequenceCueCommand::getEditor(bool isRoot)
  {
	  return new SequenceCueCommandEditor(this, isRoot);
  }
