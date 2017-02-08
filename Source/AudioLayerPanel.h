/*
  ==============================================================================

    AudioLayerPanel.h
    Created: 20 Nov 2016 3:08:49pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef AUDIOLAYERPANEL_H_INCLUDED
#define AUDIOLAYERPANEL_H_INCLUDED


#include "SequenceLayerPanel.h"
#include "AudioLayer.h"

class AudioLayerPanel :
	public SequenceLayerPanel
{
public:
	AudioLayerPanel(AudioLayer * layer);
	~AudioLayerPanel();

	AudioLayer * audioLayer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioLayerPanel)
};




#endif  // AUDIOLAYERPANEL_H_INCLUDED
