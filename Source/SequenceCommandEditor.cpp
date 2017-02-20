/*
  ==============================================================================

    SequenceCommandEditor.cpp
    Created: 20 Feb 2017 4:15:55pm
    Author:  Ben

  ==============================================================================
*/

#include "SequenceCommandEditor.h"

SequenceCommandEditor::SequenceCommandEditor(SequenceCommand * _sequenceCommand, bool isRoot) :
	InspectableEditor(_sequenceCommand, isRoot),
	sequenceCommand(_sequenceCommand)
{
	SequenceChooserUI::TargetType targetType = SequenceChooserUI::SEQUENCE;

	switch (sequenceCommand->actionType)
	{
	case SequenceCommand::PLAY_SEQUENCE:
	case SequenceCommand::PAUSE_SEQUENCE:
	case SequenceCommand::STOP_SEQUENCE:
	case SequenceCommand::TOGGLE_SEQUENCE:
		targetType = SequenceChooserUI::SEQUENCE;
		break;
	}

	chooserUI = new SequenceChooserUI(sequenceCommand->target, targetType);
	addAndMakeVisible(chooserUI);
	setSize(10, 16);
}

SequenceCommandEditor::~SequenceCommandEditor()
{
}

void SequenceCommandEditor::resized()
{
	chooserUI->setBounds(getLocalBounds().reduced(2, 0));
}
