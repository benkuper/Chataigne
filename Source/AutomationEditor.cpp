/*
  ==============================================================================

    AutomationEditor.cpp
    Created: 5 Feb 2017 2:58:27pm
    Author:  Ben

  ==============================================================================
*/

#include "AutomationEditor.h"

AutomationEditor::AutomationEditor(Automation * automation, bool isRoot) :
	GenericControllableContainerEditor(automation, isRoot),
	automation(automation),
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

void AutomationEditor::resizedInternalContent(Rectangle<int> &r)
{
	if (automationUI != nullptr)
	{
		if (automationUI != nullptr) automationUI->setBounds(r.withHeight(automationUI->getHeight()));
		r.translate(0, automationUI->getHeight());
	}
	
	r.setHeight(0);
}
