/*
  ==============================================================================

    AudioLayerClipManagerUI.h
    Created: 8 Feb 2017 12:20:16pm
    Author:  Ben

  ==============================================================================
*/

#ifndef AUDIOLAYERCLIPMANAGERUI_H_INCLUDED
#define AUDIOLAYERCLIPMANAGERUI_H_INCLUDED

#include "AudioLayerClipManager.h"
#include "AudioLayerClipUI.h"
#include "BaseManagerUI.h"


class AudioLayerClipManagerUI :
	public BaseManagerUI<AudioLayerClipManager, AudioLayerClip, AudioLayerClipUI>
{
public:
	AudioLayerClipManagerUI(AudioLayerClipManager * manager);
	~AudioLayerClipManagerUI();

	void updateContent();
};



#endif  // AUDIOLAYERCLIPMANAGERUI_H_INCLUDED
