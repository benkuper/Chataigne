/*
  ==============================================================================

	TimeCueUI.cpp
	Created: 6 Apr 2017 11:13:51am
	Author:  Ben

  ==============================================================================
*/

#include "TimeCueUI.h"

TimeCueUI::TimeCueUI(TimeCue * timeCue) :
	BaseItemMinimalUI(timeCue)
{
	bgColor = bgColor.brighter();
	setRepaintsOnMouseActivity(true);
	autoDrawHighlightWhenSelected = false;
	setSize(10, 20);
}

TimeCueUI::~TimeCueUI()
{
}

void TimeCueUI::paint(Graphics & g)
{
	Colour c = item->isSelected ? HIGHLIGHT_COLOR : bgColor;
	if (isMouseOver()) c = c.brighter();
	g.setColour(c);
	g.fillPath(drawPath);
	g.setColour(c.darker());
	g.strokePath(drawPath, PathStrokeType(1));
}

void TimeCueUI::resized()
{
	drawPath.clear();
	drawPath.startNewSubPath(getWidth() / 2.f, (float)getHeight());
	drawPath.lineTo(0, (float)getHeight() - 5);
	drawPath.lineTo(0, 0);
	drawPath.lineTo((float)getWidth(), 0);
	drawPath.lineTo((float)getWidth(), (float)getHeight() - 5);
	drawPath.closeSubPath();
	drawPath = drawPath.createPathWithRoundedCorners(1);
}

void TimeCueUI::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDown(e);
	//timeAtMouseDown = item->time->floatValue();
	//posAtMouseDown = getX();
}

void TimeCueUI::mouseDrag(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDrag(e);
	cueUIListeners.call(&TimeCueUIListener::cueDragged, this, e);
}

void TimeCueUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->time)
	{
		cueUIListeners.call(&TimeCueUIListener::cueTimeChanged, this);
	}
}
