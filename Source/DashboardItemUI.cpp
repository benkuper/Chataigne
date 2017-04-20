/*
  ==============================================================================

    DashboardItemUI.cpp
    Created: 19 Apr 2017 11:01:13pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardItemUI.h"

DashboardItemUI::DashboardItemUI(DashboardItem * item) :
	BaseItemMinimalUI(item),
	resizer(this,nullptr)
{
	addAndMakeVisible(&resizer);
	addAndMakeVisible(&grabber);

	setSize((int)item->viewUISize->getPoint().x, (int)item->viewUISize->getPoint().y);
}

DashboardItemUI::~DashboardItemUI()
{
}

void DashboardItemUI::resized()
{
	Rectangle<int> r = getLocalBounds();

	//Grabber
	grabber.setBounds(r.removeFromTop(grabberHeight));
	grabber.repaint();

	resizer.setBounds(r.removeFromBottom(10).withLeft(getWidth() - 10));

	item->viewUISize->setPoint((float)getWidth(), (float)getHeight() + 12); //should look into that +12, if not there, size shrinks between saves

}

void DashboardItemUI::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDown(e);
	if (e.mods.isLeftButtonDown())
	{
		if (e.eventComponent == &grabber) posAtMouseDown = item->viewUIPosition->getPoint();
	}
}

void DashboardItemUI::mouseDrag(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDrag(e);
	if (e.mods.isLeftButtonDown() && e.eventComponent == &grabber)
	{
		Point<float> targetPos = posAtMouseDown + e.getOffsetFromDragStart().toFloat() / Point<float>((float)getParentComponent()->getWidth(), (float)getParentComponent()->getHeight());
		item->viewUIPosition->setPoint(targetPos);
	}

}

void DashboardItemUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->viewUIPosition) itemUIListeners.call(&ItemUIListener::itemUIGrabbed, this);
}


void DashboardItemUI::Grabber::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	g.setColour(BG_COLOR.brighter(.3f));
	const int numLines = 3;
	for (int i = 0; i < numLines; i++)
	{
		float th = (i + 1)*(float)getHeight() / ((float)numLines + 1);
		g.drawLine(0, th, (float)getWidth(), th, 1);
	}
}
