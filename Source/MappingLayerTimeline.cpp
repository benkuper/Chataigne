/*
  ==============================================================================

    MappingLayerTimeline.cpp
    Created: 20 Nov 2016 3:08:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayerTimeline.h"

#include "Style.h"

MappingLayerTimeline::MappingLayerTimeline(MappingLayer * layer) :
	SequenceLayerTimeline(layer),
	mappingLayer(layer),
	automationUI(&layer->automation)
{
	bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);
	addAndMakeVisible(&automationUI);
}

MappingLayerTimeline::~MappingLayerTimeline()
{
}

void MappingLayerTimeline::resized()
{
	automationUI.setBounds(getLocalBounds());
}
