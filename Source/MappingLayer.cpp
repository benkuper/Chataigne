/*
  ==============================================================================

    MappingLayer.cpp
    Created: 17 Nov 2016 8:00:02pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayer.h"
#include "MappingLayerPanel.h"
#include "MappingLayerTimeline.h"

MappingLayer::MappingLayer(Sequence *_sequence) :
	SequenceLayer(_sequence, "New Mapping Layer")
{
	recursiveInspectionLevel = 1;

	automation.setPositionMax(sequence->totalTime->floatValue());
	addChildControllableContainer(&automation);
	addChildControllableContainer(&mapping);


	curveValue = addFloatParameter("Value", "Direct curve value of the curve at the current sequence time", 0, 0, 1);
	curveValue->isControllableFeedbackOnly = true;

	mapping.lockInputTo(curveValue);
}

MappingLayer::~MappingLayer()
{

}

void MappingLayer::loadJSONDataInternal(var data)
{
}

SequenceLayerPanel * MappingLayer::getPanel()
{
	return new MappingLayerPanel(this);
}

SequenceLayerTimeline * MappingLayer::getTimelineUI()
{
	return new MappingLayerTimeline(this);
}

void MappingLayer::sequenceTotalTimeChanged(Sequence *)
{
	automation.setPositionMax(sequence->totalTime->floatValue());
}

void MappingLayer::sequenceCurrentTimeChanged(Sequence *, float, bool)
{
	float val = automation.getValueForPosition(sequence->currentTime->floatValue());
	curveValue->setValue(val);
}
