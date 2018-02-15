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
	bgColor = AUDIO_COLOR.withSaturation(.2f).darker(1);

	cmMUI = new AudioLayerClipManagerUI(this,&layer->clipManager);
	addAndMakeVisible(cmMUI);

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