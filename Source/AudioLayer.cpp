/*
  ==============================================================================

    AudioLayer.cpp
    Created: 20 Nov 2016 3:08:41pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "AudioLayer.h"

AudioLayer::AudioLayer(Sequence * _sequence) :
	SequenceLayer(_sequence,"New Audio Layer")
{
}

AudioLayer::~AudioLayer()
{
}

void AudioLayer::loadJSONDataInternal(var data)
{
}
