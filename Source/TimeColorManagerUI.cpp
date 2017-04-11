/*
  ==============================================================================

	TimeColorManagerUI.cpp
	Created: 11 Apr 2017 11:40:47am
	Author:  Ben

  ==============================================================================
*/

#include "TimeColorManagerUI.h"

TimeColorManagerUI::TimeColorManagerUI(TimeColorManager * manager) :
	BaseManagerUI(manager->niceName,manager,false)
{
	animateItemOnAdd = false;

	transparentBG = true;
	setViewRange(0, manager->positionMax);

	resizeOnChildBoundsChanged = false;

	manager->addAsyncCoalescedContainerListener(this);
	addExistingItems();
}

TimeColorManagerUI::~TimeColorManagerUI()
{
	manager->removeAsyncContainerListener(this);
}

void TimeColorManagerUI::setViewRange(float start, float end)
{
	viewStartPos = start;
	viewEndPos = end;
	//updateROI();

	resized();
}

void TimeColorManagerUI::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	
	g.setColour(manager->currentColor->getColor());
	g.fillRect(r.removeFromBottom(20).reduced(2,7));

	manager->gradient.point1.setX(getXForPos(0));
	manager->gradient.point2.setX(getXForPos(manager->positionMax));
	g.setGradientFill(manager->gradient);
	g.fillRect(r);
}

void TimeColorManagerUI::resized()
{

	if (getParentComponent() == nullptr) return;
	if (getWidth() == 0 || getHeight() == 0) return;
	if (itemsUI.size() == 0) return;

	
	for (auto &tui : itemsUI)
	{
		placeItemUI(tui);
	}

}

void TimeColorManagerUI::addItemUIInternal(TimeColorUI * item)
{
	item->addMouseListener(this, false);
	placeItemUI(item);
}

void TimeColorManagerUI::removeItemUIInternal(TimeColorUI * item)
{
	item->removeMouseListener(this);
}

void TimeColorManagerUI::mouseDown(const MouseEvent & e)
{
	if (e.originalComponent == this)
	{
		if (e.mods.isLeftButtonDown() && e.mods.isCtrlDown())
		{
			float pos = getPosForX(e.getMouseDownX());
			manager->addColorAt(pos, manager->getColorForPosition(pos));
		}
	}
}

void TimeColorManagerUI::mouseDrag(const MouseEvent & e)
{
	if (e.originalComponent == this)
	{

	}
	else
	{
		TimeColorUI * tui = dynamic_cast<TimeColorUI *>(e.eventComponent);

		if (tui != nullptr && !tui->item->isLocked)
		{
			if (e.mods.isLeftButtonDown())
			{
				Point<int> mp = e.getEventRelativeTo(this).getPosition();
				float pos = getPosForX(mp.x);
				tui->item->position->setValue(pos);
				placeItemUI(tui);
				repaint();
			}
		}
	}
}

void TimeColorManagerUI::placeItemUI(TimeColorUI * tui)
{
	int tx = getXForPos(tui->item->position->floatValue());
	tui->setBounds(tx-10,getHeight()-20,20,20);
}

int TimeColorManagerUI::getXForPos(float time)
{
	if (viewStartPos == viewEndPos) return 0;
	return (int)jmap<float>(time, viewStartPos, viewEndPos, 0, (float)getWidth());
}

float TimeColorManagerUI::getPosForX(int tx, bool offsetStart)
{
	float viewRange = viewEndPos - viewStartPos;
	float mapStart = offsetStart ? viewStartPos : 0;
	return jmap<float>((float)tx, 0, (float)getWidth(), mapStart, mapStart + viewRange);
}

bool TimeColorManagerUI::isInView(TimeColorUI * kui)
{
	return kui->item->position->floatValue() >= viewStartPos && kui->item->position->floatValue() <= viewEndPos;
}

void TimeColorManagerUI::newMessage(const ContainerAsyncEvent & e)
{
	if (e.type == ContainerAsyncEvent::ControllableFeedbackUpdate)
	{
		if (e.targetControllable == manager->currentColor)
		{
			repaint();
		}
	}
}
