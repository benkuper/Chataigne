/*
  ==============================================================================

    MappingInputUI.cpp
    Created: 28 Oct 2016 8:11:42pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingInputEditor.h"

MappingInputEditor::MappingInputEditor(MappingInput * _input, bool isRoot) :
	GenericControllableContainerEditor(_input,isRoot),
	input(_input)
{
	input->addMappingInputListener(this);
	updateSourceUI();
}

MappingInputEditor::~MappingInputEditor()
{
	input->removeMappingInputListener(this);
}

void MappingInputEditor::updateSourceUI()
{
	if (sourceFeedbackUI != nullptr) removeChildComponent(sourceFeedbackUI);
	if (input->inputReference != nullptr)
	{
		sourceFeedbackUI = input->inputReference->createDefaultUI();
		sourceFeedbackUI->setForceFeedbackOnly(true);
		addAndMakeVisible(sourceFeedbackUI);
	}

	resized();
}

void MappingInputEditor::resizedInternalHeader(Rectangle<int>& r)
{
	if (sourceFeedbackUI != nullptr) sourceFeedbackUI->setBounds(r.removeFromRight(100).reduced(2));
	GenericControllableContainerEditor::resizedInternalHeader(r);
}


void MappingInputEditor::inputReferenceChanged(MappingInput *)
{
	updateSourceUI();
}
