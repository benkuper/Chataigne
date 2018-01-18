/*
  ==============================================================================

    TimeTriggerManagerUI.cpp
    Created: 10 Dec 2016 12:23:33pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeTriggerManagerUI.h"
#include "TriggerLayerTimeline.h"
#include "TimeCueManager.h"

TimeTriggerManagerUI::TimeTriggerManagerUI(TriggerLayerTimeline * _timeline, TimeTriggerManager * manager) :
	BaseManagerUI("Triggers", manager, false),
	timeline(_timeline)
{
	addItemText = "Add Trigger";
	animateItemOnAdd = false;
	transparentBG = true;

	addItemBT->setVisible(false);

	noItemText = "To add triggers, double click here";

	manager->selectionManager->addSelectionListener(this);

	resizeOnChildBoundsChanged = false;
	addExistingItems();
}

TimeTriggerManagerUI::~TimeTriggerManagerUI()
{
	manager->selectionManager->removeSelectionListener(this);
}

void TimeTriggerManagerUI::resized()
{
	updateContent();
	if (transformer != nullptr) transformer->updateBoundsFromKeys();
}

void TimeTriggerManagerUI::updateContent()
{
	for (auto &ttui : itemsUI)
	{
		placeTimeTriggerUI(ttui);
		if(ttui->item->isSelected) ttui->toFront(true);
		else ttui->toBack();
	}

	
}

void TimeTriggerManagerUI::placeTimeTriggerUI(TimeTriggerUI * ttui)
{
	int tx = timeline->getXForTime(ttui->item->time->floatValue());
	ttui->setBounds(tx, 0, ttui->getWidth(), getHeight());
}

void TimeTriggerManagerUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);

	if (e.eventComponent == this)
	{
		if (e.mods.isLeftButtonDown() && e.mods.isAltDown())
		{
			float time = timeline->getTimeForX(getMouseXYRelative().x);
			manager->addTriggerAt(time, getMouseXYRelative().y*1.f / getHeight());
		} else
		{
			Array<Component *> selectables;
			Array<Inspectable *> inspectables;
			for (auto &i : itemsUI) if (i->isVisible())
			{
				selectables.add(i);
				inspectables.add(i->inspectable);
			}

			if (transformer != nullptr)
			{
				removeChildComponent(transformer);
				transformer = nullptr;
			}

			if (InspectableSelector::getInstance()) InspectableSelector::getInstance()->startSelection(this, selectables, inspectables, manager->selectionManager, !e.mods.isCommandDown());
		}
	}

}

void TimeTriggerManagerUI::mouseDoubleClick(const MouseEvent & e)
{
	float time = timeline->getTimeForX(getMouseXYRelative().x);
	manager->addTriggerAt(time, getMouseXYRelative().y*1.f / getHeight());
}

bool TimeTriggerManagerUI::keyPressed(const KeyPress & e)
{
	BaseManagerUI::keyPressed(e);
	return false;
}


void TimeTriggerManagerUI::addItemFromMenu(bool isFromAddButton, Point<int> mouseDownPos)
{
	if (isFromAddButton) return;

	float time = timeline->getTimeForX(mouseDownPos.x);
	manager->addTriggerAt(time, mouseDownPos.y*1.f / getHeight());
}
void TimeTriggerManagerUI::addItemUIInternal(TimeTriggerUI * ttui)
{
	ttui->addTriggerUIListener(this);
}

void TimeTriggerManagerUI::removeItemUIInternal(TimeTriggerUI * ttui)
{
	if (transformer != nullptr)
	{
		removeChildComponent(transformer);
		transformer = nullptr;
	}

	ttui->removeTriggerUIListener(this);
}

void TimeTriggerManagerUI::timeTriggerDragged(TimeTriggerUI * ttui, const MouseEvent & e)
{
	float targetTime = ttui->timeAtMouseDown + timeline->getTimeForX(e.getOffsetFromDragStart().x, false);
	if (e.mods.isShiftDown()) targetTime = timeline->item->sequence->cueManager->getNearestCueForTime(targetTime);
	ttui->item->time->setValue(targetTime);
	repaint();
	
}

void TimeTriggerManagerUI::timeTriggerTimeChanged(TimeTriggerUI * ttui)
{
	placeTimeTriggerUI(ttui);
}

void TimeTriggerManagerUI::inspectablesSelectionChanged()
{
	if (transformer != nullptr)
	{
		removeChildComponent(transformer);
		transformer = nullptr;
	}

	Array<TimeTriggerUI *> uiSelection;
	if (manager->selectionManager->currentInspectables.size() >= 2)
	{

	}

	for (auto &i : manager->selectionManager->currentInspectables)
	{
		TimeTrigger * k = static_cast<TimeTrigger *>(i);
		if (k == nullptr) continue;
		TimeTriggerUI * kui = getUIForItem(k);
		if (kui == nullptr) return;

		uiSelection.add(kui);
	}

	if (uiSelection.size() >= 2)
	{
		transformer = new TimeTriggerMultiTransformer(this, uiSelection);
		addAndMakeVisible(transformer);
		transformer->grabKeyboardFocus(); // so no specific key has the focus for deleting
	}
}
