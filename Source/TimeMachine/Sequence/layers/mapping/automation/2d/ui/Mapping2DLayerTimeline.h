/*
  ==============================================================================

    Mapping2DLayerTimeline.h
    Created: 23 Mar 2020 5:18:10pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Mapping2DTimeline :
	public AutomationMappingLayerTimeline
{
public:
	Mapping2DTimeline(Mapping2DLayer* layer);
	~Mapping2DTimeline() {}

	Mapping2DLayer* mappingLayer;

	void paint(Graphics& g) override;

	void controllableFeedbackUpdateInternal(Controllable* c) override;
};