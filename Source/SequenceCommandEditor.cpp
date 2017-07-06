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
	case SequenceCommand::TOGGLE_SEQUENCE:
		playFromStartUI = sequenceCommand->playFromStart->createToggle();
		addAndMakeVisible(playFromStartUI);
	case SequenceCommand::PAUSE_SEQUENCE:
	case SequenceCommand::STOP_SEQUENCE:
		targetType = SequenceChooserUI::SEQUENCE;

		break;
            
        case SequenceCommand::ENABLE_LAYER:
        case SequenceCommand::DISABLE_LAYER:
            targetType = SequenceChooserUI::LAYER;
            break;
	}

	chooserUI = new SequenceChooserUI(sequenceCommand->target, targetType);
	addAndMakeVisible(chooserUI);
	
	setSize(10, (playFromStartUI != nullptr)?40:16);
}

SequenceCommandEditor::~SequenceCommandEditor()
{
}

void SequenceCommandEditor::resized()
{
	Rectangle<int> r = getLocalBounds().reduced(2, 0);
	chooserUI->setBounds(r.removeFromTop(16));
	
	if (playFromStartUI != nullptr)
	{
		r.removeFromTop(2);
		playFromStartUI->setBounds(r);
	}
}
