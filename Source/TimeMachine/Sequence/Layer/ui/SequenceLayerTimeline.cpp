/*
  ==============================================================================

    SequenceLayerTimeline.cpp
    Created: 28 Oct 2016 8:17:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerTimeline.h"

SequenceLayerTimeline::SequenceLayerTimeline(SequenceLayer * layer) :
	BaseItemMinimalUI<SequenceLayer>(layer)
{
	bgColor = item->color->getColor();

	item->sequence->addAsyncContainerListener(this);
	setSize(100, item->uiHeight->intValue());


	bringToFrontOnSelect = false;
	//setInterceptsMouseClicks(true, true);
	setWantsKeyboardFocus(false);
	setMouseClickGrabsKeyboardFocus(false);

    addAndMakeVisible(&needle);
    
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



void SequenceLayerTimeline::updateNeedlePosition()
{
    int tx = getXForTime(item->sequence->currentTime->floatValue());
    needle.setBounds(getLocalBounds().withWidth(1).withX(tx));
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
			shouldUpdateNeedle = true;
		}
	}
	else if (c == item->sequence->currentTime)
	{
		shouldUpdateNeedle = true;
	} else if (c == item->color)
	{
		bgColor = item->color->getColor();
        repaint();
	}
}

void SequenceLayerTimeline::timerCallback()
{
	if (shouldUpdateNeedle)
	{
        updateNeedlePosition();
		shouldUpdateNeedle = false;
	}
}

SequenceLayerTimeline::TimelineNeedle::TimelineNeedle() :
timeBarColor(defaultTimeBarColor)
{
    
}

SequenceLayerTimeline::TimelineNeedle::~TimelineNeedle()
{
    
}

void SequenceLayerTimeline::TimelineNeedle::paint(juce::Graphics &g)
{
    g.fillAll(timeBarColor);
}
