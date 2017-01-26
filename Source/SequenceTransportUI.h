/*
  ==============================================================================

    SequenceTransportUI.h
    Created: 20 Nov 2016 2:24:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#ifndef SEQUENCETRANSPORTUI_H_INCLUDED
#define SEQUENCETRANSPORTUI_H_INCLUDED

#include "Sequence.h"
#include "FloatParameterLabelUI.h"

class SequenceTransportUI :
	public Component
{
public:
	SequenceTransportUI(Sequence * _sequence);
	~SequenceTransportUI();

	Sequence * sequence;
	TimeLabel timeLabel;



	void paint(Graphics &g) override;
	void resized() override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceTransportUI)
};


#endif  // SEQUENCETRANSPORTUI_H_INCLUDED
