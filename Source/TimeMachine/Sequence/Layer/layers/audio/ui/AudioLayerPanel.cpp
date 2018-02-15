/*
  ==============================================================================

    AudioLayerPanel.cpp
    Created: 20 Nov 2016 3:08:49pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "AudioLayerPanel.h"

AudioLayerPanel::AudioLayerPanel(AudioLayer * layer) :
	SequenceLayerPanel(layer),
	audioLayer(layer)
{
	bgColor = AUDIO_COLOR.withSaturation(.2f).darker(1);
	
	moduleChooser.setTextWhenNoChoicesAvailable("No audio module");
	moduleChooser.setTextWhenNothingSelected("Choose an audio module");
	
	moduleChooser.filterModuleFunc = &AudioLayerPanel::isAudioModule;
	moduleChooser.buildModuleBox();

	moduleChooser.addChooserListener(this);
	moduleChooser.setModuleSelected(audioLayer->audioModule,true);

	addAndMakeVisible(&moduleChooser);

	audioLayer->addAudioLayerListener(this);

	enveloppeUI = audioLayer->enveloppe->createSlider();
	addAndMakeVisible(enveloppeUI);

}

AudioLayerPanel::~AudioLayerPanel()
{
	if (!inspectable.wasObjectDeleted()) audioLayer->removeAudioLayerListener(this);
}


void AudioLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	SequenceLayerPanel::resizedInternalContent(r);  
	Rectangle<int> gr = r.reduced(2).removeFromTop(16);

	moduleChooser.setBounds(gr.removeFromLeft(80));
	gr.removeFromLeft(5);
	enveloppeUI->setBounds(gr);
}

void AudioLayerPanel::targetAudioModuleChanged(AudioLayer *)
{
	moduleChooser.setModuleSelected(audioLayer->audioModule,true);
}

void AudioLayerPanel::selectedModuleChanged(ModuleChooserUI *, Module * m)
{
	audioLayer->setAudioModule(dynamic_cast<AudioModule *>(m));
}

void AudioLayerPanel::moduleListChanged(ModuleChooserUI *)
{
	moduleChooser.setModuleSelected(audioLayer->audioModule, true);

}
