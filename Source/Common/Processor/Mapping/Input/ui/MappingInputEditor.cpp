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
	input->addAsyncMappingInputListener(this);
	updateSourceUI();
}

MappingInputEditor::~MappingInputEditor()
{
	input->removeAsyncMappingInputListener(this);
}

void MappingInputEditor::updateSourceUI()
{
	if (sourceFeedbackUI != nullptr) removeChildComponent(sourceFeedbackUI.get());
	if (input->inputReference != nullptr)
	{
		sourceFeedbackUI.reset(input->inputReference->createDefaultUI());
		sourceFeedbackUI->showLabel = false;
		//sourceFeedbackUI->setForceFeedbackOnly(true);
		addAndMakeVisible(sourceFeedbackUI.get());
	}

	resized();
}

void MappingInputEditor::resizedInternalHeader(Rectangle<int>& r)
{
	if (sourceFeedbackUI != nullptr) sourceFeedbackUI->setBounds(r.removeFromRight(jmin(r.getWidth()-150,200)).reduced(2));
	GenericControllableContainerEditor::resizedInternalHeader(r);
}

void MappingInputEditor::newMessage(const MappingInput::MappingInputEvent & e)
{
	switch (e.type)
	{
	case MappingInput::MappingInputEvent::INPUT_REFERENCE_CHANGED:
		inputReferenceChangedAsync(e.mappingInput);
		break;

	case MappingInput::MappingInputEvent::PARAMETER_VALUE_CHANGED:
		break;
	}
}


void MappingInputEditor::inputReferenceChangedAsync(MappingInput *)
{
	updateSourceUI();
}
