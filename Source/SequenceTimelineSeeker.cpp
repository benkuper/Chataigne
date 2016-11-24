/*
  ==============================================================================

    SequenceTimelineSeeker.cpp
    Created: 23 Nov 2016 11:31:50pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceTimelineSeeker.h"
#include "Style.h"

SequenceTimelineSeeker::SequenceTimelineSeeker(Sequence * _sequence) :
	sequence(_sequence)
{
}

SequenceTimelineSeeker::~SequenceTimelineSeeker()
{
}

void SequenceTimelineSeeker::paint(Graphics & g)
{
	g.setColour(BG_COLOR.brighter(.3f));
	g.fillRoundedRectangle(getLocalBounds().toFloat(),2);
}
