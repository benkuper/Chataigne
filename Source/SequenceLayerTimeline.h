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

	int getXForTime(float time);
	float getTimeForX(int tx, bool offsetStart = true);

	virtual void updateContent() {} // to be overriden
	virtual void paintOverChildren(Graphics & g) override;

	void controllableFeedbackUpdateInternal(Controllable * c) override;



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerTimeline)
};





#endif  // SEQUENCELAYERTIMELINE_H_INCLUDED
