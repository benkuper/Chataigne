/*
  ==============================================================================

    AudioLayerTimeline.cpp
    Created: 20 Nov 2016 3:09:01pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "AudioLayerTimeline.h"

AudioLayerTimeline::AudioLayerTimeline(AudioLayer * layer) :
	SequenceLayerTimeline(layer),
	audioLayer(layer)
{

	cmMUI.reset(new AudioLayerClipManagerUI(this, &layer->clipManager));
	addAndMakeVisible(cmMUI.get());

	updateContent();
}

AudioLayerTimeline::~AudioLayerTimeline()
{

}

void AudioLayerTimeline::resized()
{
	cmMUI->setBounds(getLocalBounds());
}

void AudioLayerTimeline::updateContent()
{
	cmMUI->updateContent();
}