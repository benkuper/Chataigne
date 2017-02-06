/*
  ==============================================================================

    MappingLayerTimeline.cpp
    Created: 20 Nov 2016 3:08:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayerTimeline.h"

#include "Style.h"

MappingLayerTimeline::MappingLayerTimeline(MappingLayer * layer) :
	SequenceLayerTimeline(layer),
	automationUI(&layer->automation)
{
	bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);
	addAndMakeVisible(&automationUI);

	updateContent();
}

MappingLayerTimeline::~MappingLayerTimeline()
{
}

void MappingLayerTimeline::updateContent()
{
	automationUI.setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
}

void MappingLayerTimeline::resized()
{
	automationUI.setBounds(getLocalBounds());
}
/*
void MappingLayerTimeline::controllableFeedbackUpdateInternal(Controllable * c)
{
	
	SequenceLayerTimeline::controllableFeedbackUpdateInternal(c);
	MappingLayer * ml = static_cast<MappingLayer *>(item);

	if (c == ml->sequence->currentTime)
	{
		automationUI.setCurrentPosition(item->sequence->currentTime->floatValue());
	}
	else if (c == ml->curveValue)
	{
		automationUI.setCurrentValue(ml->curveValue->floatValue());
	}
	
}
*/