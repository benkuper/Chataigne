/*
  ==============================================================================

    AudioLayerClip.cpp
    Created: 8 Feb 2017 12:20:02pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioLayerClip.h"

AudioLayerClip::AudioLayerClip(const String &name) :
	BaseItem(name)
{
}

AudioLayerClip::~AudioLayerClip()
{
}

void AudioLayerClip::updateAudioSourceFile()
{
}

void AudioLayerClip::onContainerParameterChanged(Parameter * p)
{
	if (p == filePath)
	{
		updateAudioSourceFile();
	}
}
