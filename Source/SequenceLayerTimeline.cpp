/*
  ==============================================================================

    SequenceLayerTimeline.cpp
    Created: 28 Oct 2016 8:17:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerTimeline.h"

SequenceLayerTimeline::SequenceLayerTimeline(SequenceLayer * layer) :
	BaseItemMinimalUI<SequenceLayer>(layer)
{

	setSize(100, item->uiHeight->intValue());
}

SequenceLayerTimeline::~SequenceLayerTimeline()
{

}

void SequenceLayerTimeline::controllableFeedbackUpdateInternal(Controllable * c)
{
	
	if (c == item->uiHeight)
	{
		DBG("set size " << String(getParentComponent() != nullptr));
		setSize(getWidth(), item->uiHeight->intValue());
	}
}
