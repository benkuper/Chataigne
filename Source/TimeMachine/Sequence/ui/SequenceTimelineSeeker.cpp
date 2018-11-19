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

	//e.source.enableUnboundedMouseMovement(true, true);
	setMouseCursor(MouseCursor::NoCursor);
	//DBG("viewStartAtMouseDown : " << viewStartAtMouseDown << "/ viewEndAtMouseDown : " << viewEndAtMouseDown << "/ timeAnchorAtMouseDown : " << timeAnchorAtMouseDown);
}


void SequenceTimelineSeeker::mouseDrag(const MouseEvent & e)
{

	if (e.mods.isLeftButtonDown())
	{
		if (e.originalComponent == &handle)
		{

			if (e.getScreenPosition().x == 0 || e.getScreenPosition().y == 0) return;

			Point<float> offset = (e.getScreenPosition() - e.getMouseDownScreenPosition()).toFloat();
			//float offsetY = e.getEventRelativeTo(this).getOffsetFromDragStart().y;

			offset.x = offset.x < 0 ? jmin<float>(offset.x + minActionDistX, 0) : jmax<float>(offset.x - minActionDistX, 0);
			offset.y = offset.y< 0 ? jmin<float>(offset.y + minActionDistY, 0) : jmax<float>(offset.y - minActionDistY, 0);

			offset.y *= sequence->totalTime->floatValue();
			if (e.mods.isShiftDown()) offset.y *= 2;
			if (e.mods.isAltDown()) offset.y /= 2;
			offset.y *= zoomSensitivity;

			float viewTimeFactor = ((viewTimeAtMouseDown - offset.y) / viewTimeAtMouseDown);

			if (viewTimeFactor == 0) return;

			float startDist = timeAnchorAtMouseDown - viewStartAtMouseDown;
			float endDist = timeAnchorAtMouseDown - viewEndAtMouseDown;
			float newViewStart = timeAnchorAtMouseDown - startDist*viewTimeFactor + getTimeForX(offset.x);
			float newViewEnd = timeAnchorAtMouseDown - endDist*viewTimeFactor + getTimeForX(offset.x);

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
	//e.source.enableUnboundedMouseMovement(false, true);
	setMouseCursor(MouseCursor::NormalCursor);
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
		//repaint();
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