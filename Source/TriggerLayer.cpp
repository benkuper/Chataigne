/*
  ==============================================================================

    TriggerLayer.cpp
    Created: 17 Nov 2016 7:59:54pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "TriggerLayer.h"
#include "TriggerLayerPanel.h"
#include "TriggerLayerTimeline.h"

TriggerLayer::TriggerLayer(Sequence * _sequence, var params) :
	SequenceLayer(_sequence, "New Trigger Layer"),
	ttm(this,_sequence)
{
	addChildControllableContainer(&ttm);

	helpID = "TriggerLayer";

}

TriggerLayer::~TriggerLayer()
{
}


var TriggerLayer::getJSONData()
{
	var data = SequenceLayer::getJSONData();
	data.getDynamicObject()->setProperty("triggerManager", ttm.getJSONData());
	return data;
}

void TriggerLayer::loadJSONDataInternal(var data)
{
	SequenceLayer::loadJSONDataInternal(data);
	ttm.loadJSONData(data.getProperty("triggerManager", var()));
}

SequenceLayerPanel * TriggerLayer::getPanel()
{
	return new TriggerLayerPanel(this);
}

SequenceLayerTimeline * TriggerLayer::getTimelineUI()
{
	return new TriggerLayerTimeline(this);
}
