/*
  ==============================================================================

	MappingInputUI.cpp
	Created: 28 Oct 2016 8:11:42pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

MappingInputEditor::MappingInputEditor(MappingInput* _input, bool isRoot) :
	BaseItemEditor(_input, isRoot),
	input(_input)
{
	input->addAsyncMappingInputListener(this);
	updateSourceUI();
	triggersProcessUI.reset(input->triggersProcess->createToggle(ImageCache::getFromMemory(BinaryData::process_png, BinaryData::process_pngSize)));
	addAndMakeVisible(triggersProcessUI.get());
}

MappingInputEditor::~MappingInputEditor()
{
	input->removeAsyncMappingInputListener(this);
}

void MappingInputEditor::updateSourceUI()
{
	if (sourceFeedbackUI != nullptr) removeChildComponent(sourceFeedbackUI.get());
	if (Parameter* p = input->getInputAt(input->getPreviewIndex()))
	{
		sourceFeedbackUI.reset(p->createDefaultUI());
		if (p->type == Parameter::POINT2D)
		{
			if (DoubleSliderUI* dui = dynamic_cast<DoubleSliderUI*>(sourceFeedbackUI.get())) dui->canShowExtendedEditor = false;
		}

		sourceFeedbackUI->showLabel = false;
		//sourceFeedbackUI->setForceFeedbackOnly(true);
		addAndMakeVisible(sourceFeedbackUI.get());
	}

	resized();
}

void MappingInputEditor::resizedInternalHeaderItemInternal(Rectangle<int>& r)
{
	triggersProcessUI->setBounds(r.removeFromRight(r.getHeight()).reduced(2));
	if (sourceFeedbackUI != nullptr) sourceFeedbackUI->setBounds(r.removeFromRight(jlimit(100, 300, r.getWidth())).reduced(2));
	BaseItemEditor::resizedInternalHeaderItemInternal(r);
}

void MappingInputEditor::newMessage(const MappingInput::MappingInputEvent& e)
{
	switch (e.type)
	{
	case MappingInput::MappingInputEvent::INPUT_REFERENCE_CHANGED:
		inputReferenceChangedAsync(e.mappingInput);
		break;

	case MappingInput::MappingInputEvent::PARAMETER_VALUE_CHANGED:
		break;

	case MappingInput::MappingInputEvent::INPUT_PREVIEW_CHANGED:
		updateSourceUI();
		break;
	}
}


void MappingInputEditor::inputReferenceChangedAsync(MappingInput*)
{
	updateSourceUI();
}
