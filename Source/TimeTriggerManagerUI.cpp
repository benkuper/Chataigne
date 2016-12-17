/*
  ==============================================================================

    TimeTriggerManagerUI.cpp
    Created: 10 Dec 2016 12:23:33pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeTriggerManagerUI.h"
#include "TriggerLayerTimeline.h"

TimeTriggerManagerUI::TimeTriggerManagerUI(TriggerLayerTimeline * _timeline, TimeTriggerManager * manager) :
	BaseManagerUI("Triggers", manager, false),
	timeline(_timeline)
{
	addItemText = "Add Trigger";
	transparentBG = true;

	addExistingItems();
}

TimeTriggerManagerUI::~TimeTriggerManagerUI()
{
}

void TimeTriggerManagerUI::resized()
{
	updateContent();
}

void TimeTriggerManagerUI::updateContent()
{

	for (auto &ttui : itemsUI)
	{
		placeTimeTriggerUI(ttui);
		if(ttui->tt->isSelected) ttui->toFront(true);
		else ttui->toBack();
	}
}

void TimeTriggerManagerUI::placeTimeTriggerUI(TimeTriggerUI * ttui)
{
	int tx = timeline->getXForTime(ttui->tt->time->floatValue());
	ttui->setBounds(tx, 0, ttui->getWidth(), getHeight());
	
}

void TimeTriggerManagerUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if (e.mods.isLeftButtonDown())
	{
		if (e.mods.isCtrlDown())
		{
			float time = timeline->getTimeForX(getMouseXYRelative().x);
			manager->addTriggerAt(time);
		}
	}
}

void TimeTriggerManagerUI::addItemFromMenu(Point<int> mouseDownPos)
{
	float time = timeline->getTimeForX(mouseDownPos.x);
	manager->addTriggerAt(time);
}

void TimeTriggerManagerUI::addItemUIInternal(TimeTriggerUI * ttui)
{
	ttui->addTriggerUIListener(this);
}

void TimeTriggerManagerUI::removeItemUIInternal(TimeTriggerUI * ttui)
{
	ttui->removeTriggerUIListener(this);
}

void TimeTriggerManagerUI::timeTriggerDragged(TimeTriggerUI * ttui, const MouseEvent & e)
{
	if (!e.mods.isShiftDown())
	{
		float targetTime = ttui->timeAtMouseDown + timeline->getTimeForX(e.getOffsetFromDragStart().x, false);
		ttui->tt->time->setValue(targetTime);
	}
	repaint();
	
}

void TimeTriggerManagerUI::timeTriggerTimeChanged(TimeTriggerUI * ttui)
{
	placeTimeTriggerUI(ttui);
}
