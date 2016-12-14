/*
  ==============================================================================

    MappingLayerTimeline.h
    Created: 20 Nov 2016 3:08:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef MAPPINGLAYERTIMELINE_H_INCLUDED
#define MAPPINGLAYERTIMELINE_H_INCLUDED

#include "SequenceLayerTimeline.h"
#include "MappingLayer.h"
#include "AutomationUI.h"

class MappingLayerTimeline :
	public SequenceLayerTimeline
{
public:
	MappingLayerTimeline(MappingLayer * layer);
	~MappingLayerTimeline();

	AutomationUI automationUI;

	void updateContent() override;
	void resized();
	
	void controllableFeedbackUpdateInternal(Controllable * c) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingLayerTimeline)

};


#endif  // MAPPINGLAYERTIMELINE_H_INCLUDED
