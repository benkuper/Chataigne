/*
  ==============================================================================

    MappingLayerTimeline.cpp
    Created: 20 Nov 2016 3:08:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayerTimeline.h"

MappingLayerTimeline::MappingLayerTimeline(MappingLayer * layer, Component * component) :
	SequenceLayerTimeline(layer),
	mappingLayer(layer)
{
	setInterceptsMouseClicks(true, true);
	
	mainComponent.reset(component);
	addAndMakeVisible(mainComponent.get());

	needle.toFront(false);
	updateContent();
}

MappingLayerTimeline::~MappingLayerTimeline()
{
}

void MappingLayerTimeline::resized()
{
	mainComponent->setBounds(getLocalBounds());
}