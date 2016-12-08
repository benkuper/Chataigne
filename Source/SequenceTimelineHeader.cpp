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
	addAndMakeVisible(needle);

	setSize(6, 20);
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
			g.drawVerticalLine(tx, 0, (float)getHeight());
			g.setFont(10);
			g.drawText(String(i), tx+2, 2, 20, 10, Justification::topLeft);
		} else
		{
			g.setColour(BG_COLOR.darker(.2f));
			g.drawVerticalLine(tx, 0, (float)getHeight());
		}

		
	}
}

void SequenceTimelineHeader::resized()
{
	Rectangle<int> r = getLocalBounds();
	Rectangle<int> nr = r.withSize(10, getHeight());
	nr.setPosition(getXForTime(sequence->currentTime->floatValue())-needle.getWidth()/2, 0);
	needle.setBounds(nr);
}

void SequenceTimelineHeader::mouseDown(const MouseEvent & e)
{
	DBG("mosue down !");
	if (e.mods.isLeftButtonDown())
	{
		sequence->currentTime->setValue(getTimeForX(e.getPosition().x));
	}
}

void SequenceTimelineHeader::mouseDrag(const MouseEvent & e)
{
	if(e.mods.isLeftButtonDown())
	{
		sequence->currentTime->setValue(getTimeForX(e.getPosition().x));
	}
}

int SequenceTimelineHeader::getXForTime(float time)
{
	float viewStart = sequence->viewStartTime->floatValue();
	float viewEnd = sequence->viewEndTime->floatValue();
	return (int)jmap<float>(time, viewStart, viewEnd, 0, (float)getWidth());
}

float SequenceTimelineHeader::getTimeForX(int tx)
{
	float viewStart = sequence->viewStartTime->floatValue();
	float viewEnd = sequence->viewEndTime->floatValue();
	return jmap<float>((float)tx, 0, (float)getWidth(), viewStart, viewEnd);
}

void SequenceTimelineHeader::newMessage(const ContainerAsyncEvent & e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:

		if (e.targetControllable == sequence->viewStartTime || e.targetControllable == sequence->viewEndTime)
		{
			repaint();
			resized();
		} else if (e.targetControllable == sequence->currentTime)
		{
			resized();
		} else if (e.targetControllable == sequence->totalTime)
		{
			resized();
		}
		break;
		
	default:
	//other events not handled 
	break;
	}
}

void TimeNeedleUI::paint(Graphics & g)
{
	g.setColour(HIGHLIGHT_COLOR);
	Path p;
	p.addRectangle(getWidth() / 2 - 1, 0, 2, getHeight());
	p.addTriangle(0, 0, getWidth(), 0, getWidth()/2.f, 4);
	p.addTriangle(0, getHeight(), getWidth() / 2, getHeight() - 4, getWidth(), getHeight());
	g.fillPath(p);
}
