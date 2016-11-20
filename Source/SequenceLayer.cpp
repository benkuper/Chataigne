/*
  ==============================================================================

    SequenceLayer.cpp
    Created: 28 Oct 2016 8:15:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayer.h"

SequenceLayer::SequenceLayer(const String &name) :
	BaseItem(name)
{
	uiHeight = addIntParameter("UI Height", "Height of the layer in the editor", 60, 20, 300);
}

SequenceLayer::~SequenceLayer()
{
	sequence->removeSequenceListener(this);
}

void SequenceLayer::setSequence(Sequence * _sequence)
{
	jassert(sequence != nullptr);

	sequence = _sequence;
	sequence->addSequenceListener(this);

	init();
}

var SequenceLayer::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	return data;
}

void SequenceLayer::sequenceCurrentTimeChanged(Sequence *, bool /*evaluateSkippedData*/)
{

}

