/*
  ==============================================================================

    AutomationEditor.cpp
    Created: 5 Feb 2017 2:58:27pm
    Author:  Ben

  ==============================================================================
*/

#include "AutomationEditor.h"

AutomationEditor::AutomationEditor(Automation * automation, bool isRoot) :
	InspectableEditor(automation, isRoot),
	automationUI(nullptr)
{
	if (automation->showUIInEditor)
	{
		automationUI = new AutomationUI(automation);
		automationUI->bgColor = BG_COLOR;
		automationUI->transparentBG = false;

		addAndMakeVisible(automationUI);
		automationUI->setViewRange(0, automation->positionMax);
		setSize(100, 100);

	}
}

AutomationEditor::~AutomationEditor()
{
}

void AutomationEditor::resized()
{
	if (automationUI != nullptr) automationUI->setBounds(getLocalBounds());
}
