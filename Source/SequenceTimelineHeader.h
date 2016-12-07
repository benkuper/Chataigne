/*
  ==============================================================================

    SequenceTimelineHeader.h
    Created: 23 Nov 2016 11:32:15pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCETIMELINEHEADER_H_INCLUDED
#define SEQUENCETIMELINEHEADER_H_INCLUDED

#include "Sequence.h"

class SequenceTimelineHeader :
	public Component,
	public ControllableContainer::ContainerAsyncListener
{
public:
	SequenceTimelineHeader(Sequence * _sequence);
	~SequenceTimelineHeader();

	Sequence * sequence;

	void paint(Graphics &g) override;
	void resized() override;

	int getXForTime(float time);

	void newMessage(const ContainerAsyncEvent &e) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceTimelineHeader)

};





#endif  // SEQUENCETIMELINEHEADER_H_INCLUDED
