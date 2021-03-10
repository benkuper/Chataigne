/*
  ==============================================================================

    MappingLayerTimeline.h
    Created: 20 Nov 2016 3:08:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

class MappingLayerTimeline :
	public SequenceLayerTimeline
{
public:
	MappingLayerTimeline(MappingLayer * layer, Component * mainComponent);
	~MappingLayerTimeline();

	MappingLayer * mappingLayer;

	std::unique_ptr<Component> mainComponent;

	void resized() override;
};