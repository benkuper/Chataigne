/*
  ==============================================================================

    AudioLayerClipUI.h
    Created: 8 Feb 2017 12:20:09pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUDIOLAYERCLIPUI_H_INCLUDED
#define AUDIOLAYERCLIPUI_H_INCLUDED

#include "AudioLayerClip.h"
#include "BaseItemUI.h"

class AudioLayerClipUI :
	public BaseItemUI<AudioLayerClip>
{
public:
	AudioLayerClipUI(AudioLayerClip * clip);
	~AudioLayerClipUI();

	AudioLayerClip * clip;
};




#endif  // AUDIOLAYERCLIPUI_H_INCLUDED
