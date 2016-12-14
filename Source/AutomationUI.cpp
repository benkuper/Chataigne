/*
  ==============================================================================

    AutomationUI.cpp
    Created: 11 Dec 2016 1:22:02pm
    Author:  Ben

  ==============================================================================
*/

#include "AutomationUI.h"

AutomationUI::AutomationUI(Automation * _automation) :
	BaseManagerUI("Automation",_automation,false),
	firstROIKey(0),lastROIKey(0), currentPosition(0),
	currentUI(nullptr)
{
	transparentBG = true;
	useDefaultMenu = false;
	setViewRange(0, manager->positionMax);

}

AutomationUI::~AutomationUI()
{
}

void AutomationUI::setCurrentPosition(float val)
{
	currentPosition = val;
	currentUI = getClosestKeyUIForPos(currentPosition);
	if (currentUI != nullptr)
	{
		currentUI->inspectable->selectThis();
	}
}

void AutomationUI::setViewRange(float start, float end)
{
	viewStartPos = start;
	viewEndPos = end;
	updateROI();
	resized();
}

void AutomationUI::updateROI()
{
	if (itemsUI.size() == 0) return;

	int len = itemsUI.size()-1;
	for (int i = 0; i < len; i++)
	{
		itemsUI[i]->setVisible(false);
		if (isInView(itemsUI[i]))
		{
			firstROIKey = jmax<int>(i-1,0);
			break;
		}
	}

	for (int i = len; i >= 0; i--)
	{
		itemsUI[i]->setVisible(false);
		if (isInView(itemsUI[i]))
		{
			lastROIKey = jmin<int>(i + 1, len);
			break;
		}
	}

	for (int i = firstROIKey; i <= lastROIKey; i++)
	{
		itemsUI[i]->setVisible(true);
	}
	
}

void AutomationUI::paint(Graphics & g)
{
	BaseManagerUI::paint(g);

	if (itemsUI.size() < 2) return;
	
	int count = 0;
	for (int i = firstROIKey; i < lastROIKey; i++)
	{ 
			drawTransition(g, itemsUI[i], itemsUI[i + 1]);
			count++;
	}
}

void AutomationUI::drawTransition(Graphics & g, AutomationKeyUI * k1, AutomationKeyUI * k2)
{
	Point<int> p1 = k1->getBounds().getCentre();
	Point<int> p2 = k2->getBounds().getCentre();
	
	g.setColour(k1->item->isSelected ? HIGHLIGHT_COLOR : FRONT_COLOR); 
	g.drawLine(p1.x, p1.y, p2.x, p2.y, 1);
}


void AutomationUI::resized()
{
	if (itemsUI.size() == 0) return;

	for (int i = firstROIKey;i<=lastROIKey;i++)
	{
		placeKeyUI(itemsUI[i]);
	}
}

void AutomationUI::placeKeyUI(AutomationKeyUI * kui)
{
	int tx = getXForPos(kui->item->position->floatValue());
	int ty = (1 - kui->item->value->floatValue() / manager->valueMax)*getHeight();
	Rectangle<int> kr = Rectangle<int>(0, 0, AutomationKeyUI::handleSize + 10, AutomationKeyUI::handleSize + 10).withCentre(Point<int>(tx, ty));
	kui->setBounds(kr);
}


int AutomationUI::getXForPos(float time)
{
	if (viewStartPos == viewEndPos) return 0;
	return (int)jmap<float>(time, viewStartPos, viewEndPos, 0, (float)getWidth());
}

float AutomationUI::getPosForX(int tx, bool offsetStart)
{
	float viewRange = viewEndPos - viewStartPos;
	float mapStart = offsetStart ? viewStartPos: 0;
	return jmap<float>((float)tx, 0, (float)getWidth(), mapStart, mapStart + viewRange);
}

bool AutomationUI::isInView(AutomationKeyUI * kui)
{
	return kui->item->position->floatValue() >= viewStartPos && kui->item->position->floatValue() <= viewEndPos;
}

AutomationKeyUI * AutomationUI::getClosestKeyUIForPos(float pos, int start, int end)
{
	if (itemsUI.size() == 0) return nullptr;

	if (start == -1) start = 0;
	if (end == -1) end = itemsUI.size() - 1;


	if (pos < itemsUI[0]->item->position->floatValue()) return itemsUI[0];
	if (pos > itemsUI[itemsUI.size() - 1]->item->position->floatValue()) return itemsUI[itemsUI.size() - 1];

	
	if (end-start <= 1) return itemsUI[start];
	
	int midIndex = (int)floor((start + end) / 2);
	float medPos = itemsUI[midIndex]->item->position->floatValue();

	if (pos == medPos) return itemsUI[midIndex];

	else if (pos > medPos)
	{
		return getClosestKeyUIForPos(pos,midIndex, end);
	}
	else
	{
		return getClosestKeyUIForPos(pos,start, midIndex);
	}
}

void AutomationUI::itemAdded(AutomationKey * k)
{
	BaseManagerUI::itemAdded(k);
	updateROI();
	resized();
}

void AutomationUI::itemsReordered()
{
	BaseManagerUI::itemsReordered();
	updateROI();
	repaint();
}

void AutomationUI::addItemUIInternal(AutomationKeyUI * kui)
{
	kui->addMouseListener(this, false);
}

void AutomationUI::removeItemUIInternal(AutomationKeyUI * kui)
{
	kui->removeMouseListener(this);
}

void AutomationUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);

	if (e.eventComponent == this)
	{
		if (e.mods.isCtrlDown())
		{
			manager->addItem(getPosForX(e.getPosition().x), (1 - e.getPosition().y*1.f / getHeight())*manager->valueMax);
		}
	}
	
}

void AutomationUI::mouseDrag(const MouseEvent & e)
{
	if (e.originalComponent != this)
	{
		AutomationKeyUI * kui = dynamic_cast<AutomationKeyUI *>(e.originalComponent);
		if (kui != nullptr)
		{
			Point<int> mp = e.getEventRelativeTo(this).getPosition();
			float pos = getPosForX(mp.x);
			float val = (1 - mp.y*1.f / getHeight())*manager->valueMax;
			kui->item->position->setValue(pos);
			kui->item->value->setValue(val);
			placeKeyUI(kui);
			repaint();
		}
	}
}

