/*
  ==============================================================================

    TriggerLayerTimeline.h
    Created: 20 Nov 2016 3:08:29pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "../../../ui/SequenceLayerTimeline.h"
#include "../TriggerLayer.h"
#include "TimeTriggerManagerUI.h"

class TriggerLayerTimeline :
	public SequenceLayerTimeline
{
public:
	TriggerLayerTimeline(TriggerLayer * layer);
	~TriggerLayerTimeline();

	std::unique_ptr<TimeTriggerManagerUI> ttmui;

	void resized() override;
	virtual void updateContent() override;
	
	
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriggerLayerTimeline)

};