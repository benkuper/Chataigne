/*
  ==============================================================================

    SequenceTimelineSeeker.h
    Created: 23 Nov 2016 11:31:50pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCETIMELINESEEKER_H_INCLUDED
#define SEQUENCETIMELINESEEKER_H_INCLUDED

#include "Sequence.h"

class SequenceTimelineSeeker :
	public Component
{
public:
	SequenceTimelineSeeker(Sequence * _sequence);
	~SequenceTimelineSeeker();

	Sequence * sequence;

	void paint(Graphics &g) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceTimelineSeeker)

};


#endif  // SEQUENCETIMELINESEEKER_H_INCLUDED
