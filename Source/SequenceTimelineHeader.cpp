/*
  ==============================================================================

    SequenceTimelineHeader.cpp
    Created: 23 Nov 2016 11:32:15pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceTimelineHeader.h"
#include "Style.h"

SequenceTimelineHeader::SequenceTimelineHeader(Sequence * _sequence) :
	sequence(_sequence)
{
	sequence->addAsyncContainerListener(this);
}

SequenceTimelineHeader::~SequenceTimelineHeader()
{
	sequence->removeAsyncContainerListener(this);
}

void SequenceTimelineHeader::paint(Graphics & g)
{
	g.setColour(BG_COLOR.darker(.8f));
	g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	
	//Draw ticks
	float start = floorf(sequence->viewStartTime->floatValue());
	float end = floorf(sequence->viewEndTime->floatValue());

	int numDivisions = 2;

	for (float i = start; i <= end; i+= 1.f/numDivisions)
	{
		int tx = getXForTime(i);

		if ((int)i == i) //is a tick on a second
		{
			g.setColour(BG_COLOR);
			//g.drawLine(tx, 0, tx, getHeight(), 1);
			g.drawVerticalLine(tx, 0, getHeight());
			g.setFont(10);
			g.drawText(String(i), tx+2, 2, 20, 10, Justification::topLeft);
		} else
		{
			g.setColour(BG_COLOR.darker(.2f));
			g.drawVerticalLine(tx, 0, getHeight());
		}

		
	}
}

void SequenceTimelineHeader::resized()
{
}

int SequenceTimelineHeader::getXForTime(float time)
{
	float viewStart = sequence->viewStartTime->floatValue();
	float viewEnd = sequence->viewEndTime->floatValue();
	return jmap<float>(time, viewStart, viewEnd, 0, getWidth());
}

void SequenceTimelineHeader::newMessage(const ContainerAsyncEvent & e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:

		if (e.targetControllable == sequence->viewStartTime || e.targetControllable == sequence->viewEndTime)
		{
			repaint();
		} else if (e.targetControllable == sequence->currentTime)
		{

		} else if (e.targetControllable == sequence->totalTime)
		{

		}
		break;
	}
}

