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
#include "MappingLayerEditor.h"

MappingLayer::MappingLayer(Sequence *_sequence) :
	SequenceLayer(_sequence, "New Mapping Layer")
{
	canInspectChildContainers = true;

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

var MappingLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();
	data.getDynamicObject()->setProperty("automation", automation.getJSONData());
	return data;
}

void MappingLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	automation.loadJSONData(data.getProperty("automation", var()));
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
	automation.position->setValue(sequence->currentTime->floatValue());
	curveValue->setValue(automation.value->floatValue());
}

InspectableEditor * MappingLayer::getEditor(bool isRoot)
{
	return new MappingLayerEditor(this,isRoot);
}
