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
	ttmui = new TimeTriggerManagerUI(this, &layer->ttm);
	addAndMakeVisible(ttmui);
}

TriggerLayerTimeline::~TriggerLayerTimeline()
{
}

void TriggerLayerTimeline::resized()
{
	ttmui->setBounds(getLocalBounds());
}

void TriggerLayerTimeline::updateContent()
{
	ttmui->updateContent();
}