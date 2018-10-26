/*
  ==============================================================================

    CommandTemplateEditor.cpp
    Created: 31 May 2018 11:29:07am
    Author:  Ben

  ==============================================================================
*/

#include "CommandTemplateEditor.h"

CommandTemplateEditor::CommandTemplateEditor(CommandTemplate * t, bool isRoot) :
	GenericControllableContainerEditor(t, isRoot)
{
	triggerUI = t->triggerTrigger->createButtonUI();
	addAndMakeVisible(triggerUI);
}

CommandTemplateEditor::~CommandTemplateEditor()
{
}

void CommandTemplateEditor::resizedInternalHeader(Rectangle<int>& r)
{
	triggerUI->setBounds(r.removeFromRight(60).reduced(2));
	r.removeFromRight(2);

	GenericControllableContainerEditor::resizedInternalHeader(r);
}
