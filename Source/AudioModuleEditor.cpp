/*
  ==============================================================================

    AudioModuleEditor.cpp
    Created: 6 Feb 2017 8:46:49pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioModuleEditor.h"

AudioModuleEditor::AudioModuleEditor(AudioModule * module, bool isRoot) :
	ModuleEditor(module, isRoot),
	audioModule(module),
	selector(module->am, 0, 16, 0, 16, false, false, false, false)
{
	addAndMakeVisible(&selector);
	selector.setSize(100, 300);

	gainSlider = audioModule->gain->getEditor(false);
	addAndMakeVisible(gainSlider);
	gainSlider->setSize(100, 16);

	thresholdSlider = audioModule->activityThreshold->getEditor(false);
	addAndMakeVisible(thresholdSlider);
	thresholdSlider->setSize(100, 16);

	keepToggle = audioModule->keepLastDetectedValues->getEditor(false);
	addAndMakeVisible(keepToggle);
	keepToggle->setSize(100, 16);


	valuesCCEditor = audioModule->valuesCC.getEditor(false);
	addAndMakeVisible(valuesCCEditor);
}

AudioModuleEditor::~AudioModuleEditor()
{
}

void AudioModuleEditor::resizedInternalContent(Rectangle<int>& r)
{
	if (gainSlider != nullptr)
	{
		gainSlider->setBounds(r.withHeight(gainSlider->getHeight()));
		r.translate(0, gainSlider->getHeight() + 2);
		thresholdSlider->setBounds(r.withHeight(thresholdSlider->getHeight()));
		r.translate(0, thresholdSlider->getHeight() + 2);
		keepToggle->setBounds(r.withHeight(keepToggle->getHeight()));
		r.translate(0, keepToggle->getHeight() + 2);
	}

	if (valuesCCEditor != nullptr)
	{
		valuesCCEditor->setBounds(r.withHeight(valuesCCEditor->getHeight()));
		r.translate(0, valuesCCEditor->getHeight() + 5);
	}
	
	r.setHeight(selector.getHeight());
	selector.setBounds(r);
}
