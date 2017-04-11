/*
  ==============================================================================

    TimeColorUI.cpp
    Created: 11 Apr 2017 11:40:31am
    Author:  Ben

  ==============================================================================
*/

#include "TimeColorUI.h"

TimeColorUI::TimeColorUI(TimeColor * item) :
	BaseItemMinimalUI(item)
{
	bgColor = item->color->getColor();
}

TimeColorUI::~TimeColorUI()
{
}

void TimeColorUI::paint(Graphics & g)
{
	DBG("repaint " << getLocalBounds().toString());
	g.fillAll(bgColor);
	g.setColour(Colours::white);
	g.drawRect(getLocalBounds());
}

void TimeColorUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->color)
	{
		bgColor = item->color->getColor();
		repaint();
	}
}
