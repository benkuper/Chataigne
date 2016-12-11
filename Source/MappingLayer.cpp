/*
  ==============================================================================

    MappingLayer.cpp
    Created: 17 Nov 2016 8:00:02pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayer.h"
#include "MappingLayerPanel.h"
#include "MappingLayerTimeline.h"

MappingLayer::MappingLayer(Sequence *_sequence) :
	SequenceLayer(_sequence, "New Mapping Layer")
{
	automation.positionMax = sequence->totalTime->floatValue();
}

MappingLayer::~MappingLayer()
{
}


void MappingLayer::loadJSONDataInternal(var data)
{
}

SequenceLayerPanel * MappingLayer::getPanel()
{
	return new MappingLayerPanel(this);
}

SequenceLayerTimeline * MappingLayer::getTimelineUI()
{
	return new MappingLayerTimeline(this);
}

void MappingLayer::sequenceTotalTimeChanged(Sequence *)
{
	automation.positionMax = sequence->totalTime->floatValue();
}
