/*
  ==============================================================================

    SequenceLayer.cpp
    Created: 28 Oct 2016 8:15:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayer.h"
#include "ui/SequenceLayerPanel.h"
#include "ui/SequenceLayerTimeline.h"


SequenceLayer::SequenceLayer(Sequence * _sequence, const String &name) :
	BaseItem(name,true),
	sequence(_sequence)
{
	jassert(sequence != nullptr);

	itemDataType = "SequenceLayer";

	uiHeight = addIntParameter("UI Height", "Height of the layer in the editor", 60, 30, 300);
	color = new ColorParameter("Layer Color", "Color of the layer", Colours::grey);
	addParameter(color);

	sequence = _sequence;
	sequence->addSequenceListener(this);

}

SequenceLayer::~SequenceLayer()
{
	if(sequence != nullptr) sequence->removeSequenceListener(this);
}

SequenceLayerPanel * SequenceLayer::getPanel()
{
	return new SequenceLayerPanel(this);
}

SequenceLayerTimeline * SequenceLayer::getTimelineUI()
{
	return new SequenceLayerTimeline(this);
}

//InspectableEditor * SequenceLayer::getEditor(bool isRoot)
//{
//	return new BaseItemEditor(this, isRoot);
//}

