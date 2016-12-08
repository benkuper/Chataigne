/*
  ==============================================================================

    TriggerLayerTimeline.cpp
    Created: 20 Nov 2016 3:08:29pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "TriggerLayerTimeline.h"
#include "Style.h"

TriggerLayerTimeline::TriggerLayerTimeline(TriggerLayer * layer) :
	SequenceLayerTimeline(layer),
	triggerLayer(layer)
{
}

TriggerLayerTimeline::~TriggerLayerTimeline()
{
}
