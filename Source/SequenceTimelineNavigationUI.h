/*
  ==============================================================================

    SequenceTimelineNavigationUI.h
    Created: 20 Nov 2016 2:25:19pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCETIMELINENAVIGATIONUI_H_INCLUDED
#define SEQUENCETIMELINENAVIGATIONUI_H_INCLUDED

#include "Sequence.h"

class SequenceTimelineNavigationUI :
	public Component
{
public:
	SequenceTimelineNavigationUI(Sequence * _sequence);
	~SequenceTimelineNavigationUI();

	Sequence * sequence;

	void paint(Graphics &g) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceTimelineNavigationUI)
};





#endif  // SEQUENCETIMELINENAVIGATIONUI_H_INCLUDED
