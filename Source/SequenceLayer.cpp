/*
  ==============================================================================

    SequenceLayer.cpp
    Created: 28 Oct 2016 8:15:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayer.h"

SequenceLayer::SequenceLayer() :
	BaseItem("layer")
{
}

SequenceLayer::~SequenceLayer()
{
}

void SequenceLayer::setSequence(Sequence * _sequence)
{
	jassert(sequence != nullptr);

	sequence = _sequence;
	sequence->addSequenceListener(this);
}

void SequenceLayer::sequenceCurrentTimeChanged(Sequence *, bool /*evaluateSkippedData*/)
{

}

