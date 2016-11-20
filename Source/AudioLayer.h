/*
  ==============================================================================

    AudioLayer.h
    Created: 20 Nov 2016 3:08:41pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef AUDIOLAYER_H_INCLUDED
#define AUDIOLAYER_H_INCLUDED



#include "SequenceLayer.h"

class AudioLayer :
	public SequenceLayer
{
public:
	AudioLayer();
	~AudioLayer();


	void init() override;

	void loadJSONDataInternal(var data) override;


	static AudioLayer * create() { return new AudioLayer(); }
	virtual String getTypeString() const override { return "Audio"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioLayer)
};


#endif  // AUDIOLAYER_H_INCLUDED
