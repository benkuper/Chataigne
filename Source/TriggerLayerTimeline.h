/*
  ==============================================================================

    TriggerLayerTimeline.h
    Created: 20 Nov 2016 3:08:29pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef TRIGGERLAYERTIMELINE_H_INCLUDED
#define TRIGGERLAYERTIMELINE_H_INCLUDED

#include "SequenceLayerTimeline.h"
#include "TriggerLayer.h"

class TriggerLayerTimeline :
	public SequenceLayerTimeline
{
public:
	TriggerLayerTimeline(TriggerLayer * layer);
	~TriggerLayerTimeline();

	TriggerLayer * triggerLayer;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TriggerLayerTimeline)

};


#endif  // TRIGGERLAYERTIMELINE_H_INCLUDED
