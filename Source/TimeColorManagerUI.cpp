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
	setShowAddButton(false);
	animateItemOnAdd = false;

	transparentBG = true;
	setViewRange(0, manager->positionMax);

	resizeOnChildBoundsChanged = false;

	//manager->addAsyncCoalescedContainerListener(this);
	manager->addColorManagerListener(this);
	addExistingItems();
}

TimeColorManagerUI::~TimeColorManagerUI()
{
	manager->removeColorManagerListener(this);
	//manager->removeAsyncContainerListener(this);
}

void TimeColorManagerUI::setViewRange(float start, float end)
{
	viewStartPos = start;
	viewEndPos = end;

	resized();
}

void TimeColorManagerUI::paint(Graphics & g)
{
	BaseManagerUI::paint(g);

	Rectangle<int> r = getLocalBounds();
	
	g.setColour(manager->currentColor->getColor());
	g.fillRect(r.removeFromBottom(20).reduced(2,7));

	manager->gradient.point1.setX(getXForPos(0));
	manager->gradient.point2.setX(getXForPos(manager->positionMax));

	g.fillCheckerBoard(r.toFloat(), 12, 12, Colours::white, Colours::white.darker(.2f));
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
		tui->setVisible(isInView(tui));
		if(tui->isVisible()) placeItemUI(tui);
	}

}

void TimeColorManagerUI::addItemUIInternal(TimeColorUI * item)
{
	item->addMouseListener(this, true);
	placeItemUI(item);
}

void TimeColorManagerUI::removeItemUIInternal(TimeColorUI * item)
{
	item->removeMouseListener(this);
}

void TimeColorManagerUI::mouseDoubleClick(const MouseEvent & e)
{
	if (e.originalComponent == this)
	{
		float pos = getPosForX(e.getMouseDownX());
		manager->addColorAt(pos, manager->getColorForPosition(pos));
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
		if (tui == nullptr)
		{
			tui = dynamic_cast<TimeColorUI *>(e.eventComponent->getParentComponent());
		}

		if (tui != nullptr)
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
	tui->setBounds(tx-6,getHeight()-18,12,16);
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

void TimeColorManagerUI::gradientUpdated()
{
	repaint();
}
