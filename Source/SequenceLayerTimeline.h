/*
  ==============================================================================

    SequenceLayerTimeline.h
    Created: 28 Oct 2016 8:17:29pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef SEQUENCELAYERTIMELINE_H_INCLUDED
#define SEQUENCELAYERTIMELINE_H_INCLUDED


#include "BaseItemUI.h"
#include "SequenceLayer.h"

class SequenceLayerTimeline :
	public BaseItemMinimalUI<SequenceLayer>
{
public:
	SequenceLayerTimeline(SequenceLayer *);
	virtual ~SequenceLayerTimeline();


	void controllableFeedbackUpdateInternal(Controllable * c) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerTimeline)
};





#endif  // SEQUENCELAYERTIMELINE_H_INCLUDED
