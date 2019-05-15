/*
  ==============================================================================

    AudioLayerTimeline.h
    Created: 20 Nov 2016 3:09:01pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef AUDIOLAYERTIMELINE_H_INCLUDED
#define AUDIOLAYERTIMELINE_H_INCLUDED

#include "../../../ui/SequenceLayerTimeline.h"
#include "AudioLayerClipManagerUI.h"
#include "../AudioLayer.h"

class AudioLayerTimeline :
	public SequenceLayerTimeline
{
public:
	AudioLayerTimeline(AudioLayer * layer);
	~AudioLayerTimeline();

	AudioLayer * audioLayer;
	std::unique_ptr<AudioLayerClipManagerUI> cmMUI;

	void resized() override;
	void updateContent() override;
};



#endif  // AUDIOLAYERTIMELINE_H_INCLUDED
