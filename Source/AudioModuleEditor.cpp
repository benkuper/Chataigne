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

	gainSlider = audioModule->inputGain->getEditor(false);
	addAndMakeVisible(gainSlider);
	gainSlider->setSize(100, 16);

	thresholdSlider = audioModule->activityThreshold->getEditor(false);
	addAndMakeVisible(thresholdSlider);
	thresholdSlider->setSize(100, 16);

	keepToggle = audioModule->keepLastDetectedValues->getEditor(false);
	addAndMakeVisible(keepToggle);
	keepToggle->setSize(100, 16);

	monitorVolumeSlider = audioModule->monitorVolume->getEditor(false);
	addAndMakeVisible(monitorVolumeSlider);
	monitorVolumeSlider->setSize(100, 16);

	valuesCCEditor = audioModule->valuesCC.getEditor(false);
	addAndMakeVisible(valuesCCEditor);

	audioModule->addAudioModuleListener(this);

	updateMonitorSetup();
}

AudioModuleEditor::~AudioModuleEditor()
{
	audioModule->removeAudioModuleListener(this);
}

void AudioModuleEditor::updateMonitorSetup()
{
	monitorOutChannels.clear();
	for (int i = 0; i < audioModule->monitorOutChannels.size(); i++)
	{
		InspectableEditor * b = audioModule->monitorOutChannels[i]->getEditor(false);
		addAndMakeVisible(b);
		monitorOutChannels.add(b);
	}

	resized();
}

void AudioModuleEditor::monitorSetupChanged()
{
	updateMonitorSetup();
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
		r.translate(0, keepToggle->getHeight() + 10);

		monitorVolumeSlider->setBounds(r.withHeight(monitorVolumeSlider->getHeight()));
		r.translate(0, monitorVolumeSlider->getHeight() + 2);

		for (int i = 0; i < monitorOutChannels.size(); i++)
		{
			InspectableEditor * b = monitorOutChannels[i];
			b->setBounds(r.withHeight(b->getHeight()));
			r.translate(0, b->getHeight() + 2);

		}
	}

	if (valuesCCEditor != nullptr)
	{
		valuesCCEditor->setBounds(r.withHeight(valuesCCEditor->getHeight()));
		r.translate(0, valuesCCEditor->getHeight() + 5);
	}
	
	r.setHeight(selector.getHeight());
	selector.setBounds(r);
}
