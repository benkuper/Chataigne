/*
  ==============================================================================

    TriggerLayer.cpp
    Created: 17 Nov 2016 7:59:54pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "TriggerLayer.h"
#include "TriggerLayerPanel.h"
#include "TriggerLayerTimeline.h"

TriggerLayer::TriggerLayer(Sequence * _sequence) :
	SequenceLayer(_sequence, "New Trigger Layer"),
	ttm(_sequence)
{
	addChildControllableContainer(&ttm);
}

TriggerLayer::~TriggerLayer()
{
}


void TriggerLayer::loadJSONDataInternal(var data)
{
}

SequenceLayerPanel * TriggerLayer::getPanel()
{
	return new TriggerLayerPanel(this);
}

SequenceLayerTimeline * TriggerLayer::getTimelineUI()
{
	return new TriggerLayerTimeline(this);
}
