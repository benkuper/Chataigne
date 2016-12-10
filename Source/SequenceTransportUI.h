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

class TimeLabel : 
	public FloatParameterLabelUI
{
public:
	TimeLabel(FloatParameter * p);
	~TimeLabel();

protected:
	void valueChanged(const var &) override;
	void labelTextChanged(Label * l) override;


	String valueToTimeString(float val) const;
	float timeStringToValue(String str) const;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TimeLabel)
};

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
