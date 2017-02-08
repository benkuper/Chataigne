/*
  ==============================================================================

    AudioLayerClipManager.h
    Created: 8 Feb 2017 12:19:53pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUDIOLAYERCLIPMANAGER_H_INCLUDED
#define AUDIOLAYERCLIPMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "AudioLayerClip.h"

class AudioLayerClipManager :
	public BaseManager<AudioLayerClip>
{
public:
	AudioLayerClipManager();
	~AudioLayerClipManager();
};

#endif  // AUDIOLAYERCLIPMANAGER_H_INCLUDED
