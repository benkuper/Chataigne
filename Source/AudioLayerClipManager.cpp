/*
  ==============================================================================

    AudioLayerClipManager.cpp
    Created: 8 Feb 2017 12:19:53pm
    Author:  Ben

  ==============================================================================
*/

#include "AudioLayerClipManager.h"

AudioLayerClipManager::AudioLayerClipManager() :
	BaseManager("Clip Manager")
{
}

AudioLayerClipManager::~AudioLayerClipManager()
{
}

void AudioLayerClipManager::addClipAt(float time)
{
	AudioLayerClip * t = new AudioLayerClip(time);
	BaseManager::addItem(t);
}

AudioLayerClip * AudioLayerClipManager::getClipAtTime(float time)
{
	for (auto &c : items) if (c->isInRange(time)) return c;
	return nullptr;
}
