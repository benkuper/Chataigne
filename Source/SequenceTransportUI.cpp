/*
  ==============================================================================

    SequenceTransportUI.cpp
    Created: 20 Nov 2016 2:24:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceTransportUI.h"

SequenceTransportUI::SequenceTransportUI(Sequence * _sequence) :
	sequence(_sequence),
	timeLabel(_sequence->currentTime)
{
	addAndMakeVisible(&timeLabel);
}

SequenceTransportUI::~SequenceTransportUI()
{
}

void SequenceTransportUI::paint(Graphics &)
{
	//g.fillAll(Colours::green);
}

void SequenceTransportUI::resized()
{
	Rectangle<int> r = getLocalBounds();
	timeLabel.setBounds(r);
}

TimeLabel::TimeLabel(FloatParameter * p) :
	FloatParameterLabelUI(p)
{
	valueChanged(parameter->value);
}

TimeLabel::~TimeLabel()
{
}

void TimeLabel::valueChanged(const var & v)
{
	String timeString = valueToTimeString(v);
	FloatParameterLabelUI::valueChanged(timeString);
}

void TimeLabel::labelTextChanged(Label *)
{
	parameter->setValue(timeStringToValue(valueLabel.getText()));
}

String TimeLabel::valueToTimeString(float timeVal) const
{
	int hours = floor<int>(timeVal / 3600);
	int minutes = floor<int>(fmodf(timeVal,3600) / 60);
	float seconds = fmodf(timeVal, 60);
	return String::formatted("%02i:%02i:%06.3f", hours, minutes, seconds);
}

float TimeLabel::timeStringToValue(String str) const
{
	StringArray sa;
	if (str.endsWithChar(':')) str += "0";
	sa.addTokens(str,":","");
	float value = 0;
	
	value += sa.strings[sa.strings.size()-1].getFloatValue();

	if (sa.strings.size() >= 2)
	{
		int numMinutes = sa.strings[sa.strings.size() - 2].getIntValue() * 60;
		value += numMinutes;
		if (sa.strings.size() >= 3)
		{
			int numHours = sa.strings[sa.strings.size() - 3].getFloatValue() * 3600;
			value += numHours;
		}
	}
	
	return value;
}
