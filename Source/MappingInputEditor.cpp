/*
  ==============================================================================

    MappingInputUI.cpp
    Created: 28 Oct 2016 8:11:42pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingInputEditor.h"

MappingInputEditor::MappingInputEditor(MappingInput * _input, bool isRoot) :
	InspectableEditor(_input,isRoot),
	input(_input)
{
	targetUI = input->inputTarget->createTargetUI();
	input->addMappingInputListener(this);
	addAndMakeVisible(targetUI);
	setSize(100, 50);
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

void MappingInputEditor::resized()
{
	Rectangle<int> r = getLocalBounds();
	Rectangle<int> sr = r.withHeight(16).reduced(2, 0);
	if (sourceFeedbackUI != nullptr)
	{
		sourceFeedbackUI->setBounds(sr.removeFromRight(jmax<int>(sr.getWidth() / 3, 60)));
		sr.removeFromRight(2);
	}
	targetUI->setBounds(sr);
}

void MappingInputEditor::inputReferenceChanged(MappingInput *)
{
	updateSourceUI();
}
