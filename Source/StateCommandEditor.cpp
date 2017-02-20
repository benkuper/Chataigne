/*
  ==============================================================================

    StateCommandEditor.cpp
    Created: 20 Feb 2017 3:19:35pm
    Author:  Ben

  ==============================================================================
*/

#include "StateCommandEditor.h"

StateCommandEditor::StateCommandEditor(StateCommand * _stateCommand, bool isRoot) :
	InspectableEditor(_stateCommand, isRoot),
	stateCommand(_stateCommand)
{
	StateChooserUI::TargetType targetType = StateChooserUI::STATE;

	switch (stateCommand->actionType)
	{
	case StateCommand::ACTIVATE_STATE:
	case StateCommand::DEACTIVATE_STATE:
	case StateCommand::TOGGLE_STATE:
		targetType = StateChooserUI::STATE;
		break;

	case StateCommand::TRIGGER_ACTION:
	case StateCommand::ENABLE_ACTION:
	case StateCommand::DISABLE_ACTION:
	case StateCommand::TOGGLE_ACTION:
		targetType = StateChooserUI::ACTION;
		break;

	case StateCommand::ENABLE_MAPPING:
	case StateCommand::DISABLE_MAPPING:
	case StateCommand::TOGGLE_MAPPING:
		targetType = StateChooserUI::MAPPING;
		break;
	}

	chooserUI = new StateChooserUI(stateCommand->target, targetType);
	addAndMakeVisible(chooserUI);
	setSize(10, 16);
}

StateCommandEditor::~StateCommandEditor()
{
}

void StateCommandEditor::resized()
{
	chooserUI->setBounds(getLocalBounds().reduced(2,0));
}
