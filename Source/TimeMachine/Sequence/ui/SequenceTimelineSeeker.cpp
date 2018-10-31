/*
  ==============================================================================

    SequenceTimelineSeeker.cpp
    Created: 23 Nov 2016 11:31:50pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceTimelineSeeker.h"


SequenceTimelineSeeker::SequenceTimelineSeeker(Sequence * _sequence) :
	sequence(_sequence)
{
	addAndMakeVisible(&handle);
	sequence->addAsyncContainerListener(this);
	handle.addMouseListener(this,false);

	startTimerHz(20);
}

SequenceTimelineSeeker::~SequenceTimelineSeeker()
{
	sequence->removeAsyncContainerListener(this);
}

#pragma warning (push)
#pragma warning(disable:4244)
void SequenceTimelineSeeker::paint(Graphics & g)
{
	g.setColour(BG_COLOR);
	g.fillRoundedRectangle(getLocalBounds().toFloat(),2);
	g.setColour(BG_COLOR.darker());
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
	g.setColour(HIGHLIGHT_COLOR);
	float tx = getXForTime(sequence->currentTime->floatValue());
	g.drawLine(tx, 0, tx, getHeight(), 2);
}

void SequenceTimelineSeeker::resized()
{
	Rectangle<int> r = getLocalBounds();
	r.setLeft(getXForTime(sequence->viewStartTime->floatValue()));
	r.setRight(getXForTime(sequence->viewEndTime->floatValue()));
	handle.setBounds(r);
}

void SequenceTimelineSeeker::mouseDown(const MouseEvent & e)
{
	if (e.mods.isLeftButtonDown())
	{
		viewStartAtMouseDown = sequence->viewStartTime->floatValue();
		viewEndAtMouseDown = sequence->viewEndTime->floatValue();
		timeAnchorAtMouseDown = getTimeForX(e.getEventRelativeTo(this).x);
		viewTimeAtMouseDown = (viewEndAtMouseDown - viewStartAtMouseDown);
	}
	else
	{
		sequence->viewStartTime->setValue(0);
		sequence->viewEndTime->setValue(sequence->totalTime->floatValue());
	}

	e.source.enableUnboundedMouseMovement(true, false);

	//DBG("viewStartAtMouseDown : " << viewStartAtMouseDown << "/ viewEndAtMouseDown : " << viewEndAtMouseDown << "/ timeAnchorAtMouseDown : " << timeAnchorAtMouseDown);
}


void SequenceTimelineSeeker::mouseDrag(const MouseEvent & e)
{

	if (e.mods.isLeftButtonDown())
	{
		if (e.originalComponent == &handle)
		{

			float offsetX = e.getOffsetFromDragStart().x;
			float offsetY = e.getOffsetFromDragStart().y;

			offsetX = offsetX < 0 ? jmin<int>(offsetX + minActionDistX, 0) : jmax<int>(offsetX - minActionDistX, 0);
			offsetY = offsetY< 0 ? jmin<int>(offsetY + minActionDistY, 0) : jmax<int>(offsetY - minActionDistY, 0);

			offsetY *= sequence->totalTime->floatValue();
			if (e.mods.isShiftDown()) offsetY *= 2;
			if (e.mods.isAltDown()) offsetY /= 2;
			offsetY *= zoomSensitivity;

			float viewTimeFactor = ((viewTimeAtMouseDown - offsetY) / viewTimeAtMouseDown);

			if (viewTimeFactor == 0) return;

			float startDist = timeAnchorAtMouseDown - viewStartAtMouseDown;
			float endDist = timeAnchorAtMouseDown - viewEndAtMouseDown;
			float newViewStart = timeAnchorAtMouseDown - startDist*viewTimeFactor + getTimeForX(offsetX);
			float newViewEnd = timeAnchorAtMouseDown - endDist*viewTimeFactor + getTimeForX(offsetX);

			//newViewEnd = jmax<float>(newViewEnd, newViewStart + 1);

			sequence->viewStartTime->setValue(newViewStart);
			sequence->viewEndTime->setValue(newViewEnd);
			
			
		}
		else
		{
			float destTime = getTimeForX(e.getPosition().x);
			sequence->viewStartTime->setValue(destTime - viewTimeAtMouseDown / 2);
			sequence->viewEndTime->setValue(destTime + viewTimeAtMouseDown / 2);
		}
	}
}

void SequenceTimelineSeeker::mouseUp(const MouseEvent & e)
{
	e.source.enableUnboundedMouseMovement(false, true);
}


int SequenceTimelineSeeker::getXForTime(float time)
{
	return (int)jmap<float>(time, 0, sequence->totalTime->floatValue(), 0, (float)getWidth());
}

float SequenceTimelineSeeker::getTimeForX(int tx)
{
	return jmap<float>((float)tx, 0, (float)getWidth(), 0, sequence->totalTime->floatValue());
}


void SequenceTimelineSeeker::newMessage(const ContainerAsyncEvent & e)
{
	switch (e.type)
	{
	case ContainerAsyncEvent::ControllableFeedbackUpdate:

		if (e.targetControllable == sequence->viewStartTime || e.targetControllable == sequence->viewEndTime)
		{
			resized();
		}
		else if (e.targetControllable == sequence->currentTime)
		{
			shouldRepaint = true;;
			resized();
		}
		else if (e.targetControllable == sequence->totalTime)
		{
			repaint();
			resized();
		}
		break;

	default:
		//other events not handled 
		break;
	}
}

void SequenceTimelineSeeker::timerCallback()
{
	if (shouldRepaint)
	{
		repaint();
		shouldRepaint = false;
	}
}


void SeekHandle::paint(Graphics & g)
{
	g.setColour(BLUE_COLOR.withAlpha(.3f));
	g.fillRoundedRectangle(getLocalBounds().toFloat(), 2);
	g.setColour(BLUE_COLOR.brighter());
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 2, 2);
}

void SeekHandle::resized()
{
}

#pragma warning (pop)