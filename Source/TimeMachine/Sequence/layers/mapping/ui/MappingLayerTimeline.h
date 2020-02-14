/*
  ==============================================================================

    MappingLayerTimeline.h
    Created: 20 Nov 2016 3:08:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "../MappingLayer.h"

class MappingLayerTimeline :
	public SequenceLayerTimeline
{
public:
	MappingLayerTimeline(MappingLayer * layer);
	~MappingLayerTimeline();

	MappingLayer * mappingLayer;

	std::unique_ptr<AutomationUI> automationUI;
	std::unique_ptr<GradientColorManagerUI> colorManagerUI;

	void setupUIForLayerMode();

	void updateContent() override;
	void resized() override;

	void controllableFeedbackUpdateInternal(Controllable * c) override;
	void inspectableSelectionChanged(Inspectable * i) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayerTimeline)

};