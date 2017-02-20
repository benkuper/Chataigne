/*
  ==============================================================================

    SequenceCommand.cpp
    Created: 20 Feb 2017 2:12:09pm
    Author:  Ben

  ==============================================================================
*/

#include "SequenceCommand.h"
#include "Sequence.h"
#include "SequenceCommandEditor.h"

SequenceCommand::SequenceCommand(SequenceModule * _module, CommandContext context, var params) :
	BaseCommand(_module, context, params),
	sequenceModule(_module)
{
	target = addTargetParameter("Target", "Target for the command");
	target->targetType = TargetParameter::CONTAINER;

	actionType = (ActionType)(int)params.getProperty("type", PLAY_SEQUENCE);
}

SequenceCommand::~SequenceCommand()
{
}

void SequenceCommand::trigger()
{
	if (target->targetContainer == nullptr) return;
	if (target->targetContainer.wasObjectDeleted()) return;

	switch (actionType)
	{
	case PLAY_SEQUENCE:
		((Sequence *)target->targetContainer.get())->playTrigger->trigger();
		break;

	case PAUSE_SEQUENCE:
		((Sequence *)target->targetContainer.get())->pauseTrigger->trigger();
		break;

	case STOP_SEQUENCE:
		((Sequence *)target->targetContainer.get())->stopTrigger->trigger();
		break;

	case TOGGLE_SEQUENCE:
		((Sequence *)target->targetContainer.get())->togglePlayTrigger->trigger();
		break;
	}
}

BaseCommand * SequenceCommand::create(ControllableContainer * module, CommandContext context, var params) {
	return new SequenceCommand((SequenceModule *)module, context, params);
}

InspectableEditor * SequenceCommand::getEditor(bool isRoot)
{
	return new SequenceCommandEditor(this, isRoot);
}
