/*
  ==============================================================================

    TriggerLayerTimeline.cpp
    Created: 20 Nov 2016 3:08:29pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "TriggerLayerTimeline.h"


TriggerLayerTimeline::TriggerLayerTimeline(TriggerLayer * layer) :
	SequenceLayerTimeline(layer)
{
	ttmui.reset(new TimeTriggerManagerUI(this, &layer->ttm));
	addAndMakeVisible(ttmui.get());

	updateContent();
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