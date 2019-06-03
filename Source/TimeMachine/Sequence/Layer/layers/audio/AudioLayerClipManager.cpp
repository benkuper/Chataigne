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

AudioLayerClip * AudioLayerClipManager::addClipAt(float time)
{
	AudioLayerClip * t = new AudioLayerClip(time);
	BaseManager::addItem(t);
    return t;
}

AudioLayerClip * AudioLayerClipManager::getClipAtTime(float time, bool includeDisabled)
{
	for (auto& c : items)
	{
		if (!includeDisabled && !c->enabled->boolValue()) continue;
		if (c->isInRange(time)) return c;
	}

	return nullptr;
}
