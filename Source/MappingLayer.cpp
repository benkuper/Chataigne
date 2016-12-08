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

MappingLayer::MappingLayer() :
	SequenceLayer("New Mapping Layer")
{
}

MappingLayer::~MappingLayer()
{
}

void MappingLayer::init()
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
