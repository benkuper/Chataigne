/*
  ==============================================================================

    SequenceLayerTimeline.cpp
    Created: 28 Oct 2016 8:17:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerTimeline.h"

SequenceLayerTimeline::SequenceLayerTimeline(SequenceLayer * layer) :
	BaseItemMinimalUI<SequenceLayer>(layer),
	timeBarColor(defaultTimeBarColor)
{
	bgColor = item->color->getColor();

	item->sequence->addAsyncContainerListener(this);
	setSize(100, item->uiHeight->intValue());


	bringToFrontOnSelect = false;
	//setInterceptsMouseClicks(true, true);
	setWantsKeyboardFocus(false);
	setMouseClickGrabsKeyboardFocus(false);

	startTimerHz(30);
}

SequenceLayerTimeline::~SequenceLayerTimeline()
{
	item->sequence->removeAsyncContainerListener(this);
}


int SequenceLayerTimeline::getXForTime(float time)
{
	float viewStart = item->sequence->viewStartTime->floatValue();
	float viewEnd = item->sequence->viewEndTime->floatValue();
	if (viewStart == viewEnd) return 0;
	return (int)jmap<float>(time, viewStart, viewEnd, 0, (float)getWidth());
}

float SequenceLayerTimeline::getTimeForX(int tx, bool offsetStart)
{
	if (getWidth() == 0) return 0; 
	
	float viewStart = item->sequence->viewStartTime->floatValue();
	float viewEnd = item->sequence->viewEndTime->floatValue();
	float viewTime = viewEnd - viewStart;
	float mapStart = offsetStart ? viewStart : 0;
;	return jmap<float>((float)tx, 0, (float)getWidth(), mapStart,mapStart+viewTime);
}



void SequenceLayerTimeline::paintOverChildren(Graphics & g)
{

	g.setColour(timeBarColor);
	g.drawVerticalLine(getXForTime(item->sequence->currentTime->floatValue()), 0, (float)getHeight());

	//g.setColour(item->color->getColor());
	//g.drawRoundedRectangle(getLocalBounds().reduced(1).toFloat(), 2, 2);

	BaseItemMinimalUI::paintOverChildren(g);
}

void SequenceLayerTimeline::mouseDown(const MouseEvent &e)
{
	if (e.mods.isLeftButtonDown() && e.eventComponent->getParentComponent() == this)
	{
		item->selectThis();
	}
}

void SequenceLayerTimeline::controllableFeedbackUpdateInternal(Controllable * c)
{
	BaseItemMinimalUI::controllableFeedbackUpdateInternal(c);

	if (c == item->uiHeight)
	{
		setSize(getWidth(), item->uiHeight->intValue());
	}
	else if (c == item->sequence->viewStartTime || c == item->sequence->viewEndTime)
	{
		if (isVisible())
		{
			updateContent();
			shouldRepaint = true;
		}
	}
	else if (c == item->sequence->currentTime)
	{
		shouldRepaint = true;
	} else if (c == item->color)
	{
		bgColor = item->color->getColor();
		shouldRepaint = true;
	}
}

void SequenceLayerTimeline::timerCallback()
{
	if (shouldRepaint)
	{
		repaint();
		shouldRepaint = false;
	}
}
