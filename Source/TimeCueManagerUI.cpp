/*
  ==============================================================================

    TimeCueManagerUI.cpp
    Created: 6 Apr 2017 11:35:25am
    Author:  Ben

  ==============================================================================
*/

#include "TimeCueManagerUI.h"
#include "SequenceTimelineHeader.h"

TimeCueManagerUI::TimeCueManagerUI(SequenceTimelineHeader * _header, TimeCueManager * manager) :
	BaseManagerUI("Cues", manager, false),
	header(_header)
{
	addItemText = "Add Cue";
	animateItemOnAdd = false;
	transparentBG = true;

	addItemBT->setVisible(false);
	addExistingItems();
}

TimeCueManagerUI::~TimeCueManagerUI()
{
}

void TimeCueManagerUI::resized()
{
	updateContent();
}

void TimeCueManagerUI::updateContent()
{
	//DBG("***");
	for (auto &tui : itemsUI)
	{
		//DBG("place " << tui->item->time->floatValue());
		placeTimeCueUI(tui);
		if(tui->item->isSelected) tui->toFront(true);
		else tui->toBack();
	}
}

bool TimeCueManagerUI::hitTest(int x, int y)
{
	DBG("hit test ");
	for (auto &i : itemsUI)
	{
		if (i->getBounds().contains(Point<int>(x,y)))
		{
			return true;
		}
	}

	return false;
}

void TimeCueManagerUI::placeTimeCueUI(TimeCueUI * ttui)
{
	int tx = header->getXForTime(ttui->item->time->floatValue());
	ttui->setBounds(tx-ttui->getWidth()/2, getHeight()/2, ttui->getWidth(), getHeight()/2);	
}

void TimeCueManagerUI::addCueAtPos(int x)
{
	float time = header->getTimeForX(getMouseXYRelative().x);
	manager->addCueAt(time);
}


void TimeCueManagerUI::addItemFromMenu(bool isFromAddButton, Point<int> mouseDownPos)
{
	if (isFromAddButton) return;

	float time = header->getTimeForX(mouseDownPos.x);
	manager->addCueAt(time);
}

void TimeCueManagerUI::addItemUIInternal(TimeCueUI * ttui)
{
	ttui->addCueUIListener(this);
	placeTimeCueUI(ttui);
}

void TimeCueManagerUI::removeItemUIInternal(TimeCueUI * ttui)
{
	ttui->removeCueUIListener(this);
}

void TimeCueManagerUI::cueDragged(TimeCueUI * ttui, const MouseEvent & e)
{
	if (!e.mods.isShiftDown())
	{
		float targetTime = header->getTimeForX(getMouseXYRelative().x);
		ttui->item->time->setValue(targetTime);
	}
	repaint();
	
}

void TimeCueManagerUI::cueTimeChanged(TimeCueUI * ttui)
{
	placeTimeCueUI(ttui);
}