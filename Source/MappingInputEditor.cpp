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
	if (input->inputTarget->enabled)
	{
		targetUI = new ModuleInputValueChooserUI(input->inputTarget, true);
		addAndMakeVisible(targetUI);
	}
	
	input->addMappingInputListener(this);
	setSize(100, 16);
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
	Rectangle<int> sr = r.reduced(2, 0);
	if (sourceFeedbackUI != nullptr)
	{
		if (targetUI != nullptr)
		{
			sourceFeedbackUI->setBounds(sr.removeFromRight(jmax<int>(sr.getWidth() / 3, 100)));
			sr.removeFromRight(2);
		}
		else
		{
			sourceFeedbackUI->setBounds(sr);
		}
	}

	if(targetUI != nullptr) targetUI->setBounds(sr);
}

void MappingInputEditor::inputReferenceChanged(MappingInput *)
{
	updateSourceUI();
}
