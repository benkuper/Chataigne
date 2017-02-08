/*
  ==============================================================================

    AudioLayer.cpp
    Created: 20 Nov 2016 3:08:41pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "AudioLayer.h"
#include "AudioLayerPanel.h"
#include "AudioLayerTimeline.h"

AudioLayer::AudioLayer(Sequence * _sequence) :
	SequenceLayer(_sequence,"New Audio Layer")
{
	addChildControllableContainer(&clipManager);
}

AudioLayer::~AudioLayer()
{
}

var AudioLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();
	data.getDynamicObject()->setProperty("clipManager", clipManager.getJSONData());
	return data;
}

void AudioLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	clipManager.loadJSONData(data.getProperty("clipManager", var()));
}

SequenceLayerPanel * AudioLayer::getPanel()
{
	return new AudioLayerPanel(this);
}

SequenceLayerTimeline * AudioLayer::getTimelineUI()
{
	return new AudioLayerTimeline(this);
}
