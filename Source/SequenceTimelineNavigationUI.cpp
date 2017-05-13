/*
  ==============================================================================

    SequenceTimelineNavigationUI.cpp
    Created: 20 Nov 2016 2:25:19pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceTimelineNavigationUI.h"


SequenceTimelineNavigationUI::SequenceTimelineNavigationUI(Sequence * _sequence) :
	sequence(_sequence),
	seeker(_sequence),
	header(_sequence)
{
	addAndMakeVisible(&seeker);
	addAndMakeVisible(&header);
}

SequenceTimelineNavigationUI::~SequenceTimelineNavigationUI()
{
}

void SequenceTimelineNavigationUI::paint(Graphics &)
{
}

void SequenceTimelineNavigationUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	
	//compensate for layerTimelineManagerUI margins
	r.removeFromLeft(4);
	r.removeFromRight(10);
	
	r.removeFromTop(4);
	seeker.setBounds(r.removeFromTop(15));
	r.reduce(0, 2);
	header.setBounds(r);
}
