/*
  ==============================================================================

	TimeTriggerUI.cpp
	Created: 10 Dec 2016 11:57:16am
	Author:  Ben

  ==============================================================================
*/

#include "TimeTriggerUI.h"

TimeTriggerUI::TimeTriggerUI(TimeTrigger * _tt) :
	BaseItemUI<TimeTrigger>(_tt, ResizeMode::NONE, false)
{

	autoDrawHighlightWhenSelected = false;
	setName(_tt->niceName);

	removeBT->setVisible(item->isSelected);
	enabledBT->setVisible(item->isSelected);
	updateSizeFromName();

}

TimeTriggerUI::~TimeTriggerUI()
{
}

void TimeTriggerUI::paint(Graphics & g)
{
	Colour c = BG_COLOR.brighter(.1f);
	if (item->isTriggered->boolValue()) c = GREEN_COLOR.darker();
	if (!item->enabled->boolValue()) c = c.darker(.6f);

	g.setColour(c);
	 
	g.fillRect(flagRect);
	g.setColour(item->isSelected?HIGHLIGHT_COLOR:c.brighter());
	g.drawRect(flagRect);
	g.drawVerticalLine(0, 0, (float)getHeight());

}

void TimeTriggerUI::resized()
{
	Rectangle<int> r = getLocalBounds();

	int ty = (int)(item->flagY->floatValue()*(getHeight() - 20));

	flagRect = r.translated(0, ty).withHeight(20);
	lineRect = r.withWidth(6);

	Rectangle<int> p = flagRect.reduced(2, 2);
	if (item->isSelected)
	{

		removeBT->setBounds(p.removeFromRight(p.getHeight()));
		p.removeFromRight(2);
		enabledBT->setBounds(p.removeFromRight(15));
		p.removeFromRight(2);
	}

	itemLabel.setBounds(p);
}

bool TimeTriggerUI::hitTest(int x, int y)
{
	if (flagRect.contains(x, y)) return true;
	if (lineRect.contains(x, y)) return true;
	return false;
}

void TimeTriggerUI::updateSizeFromName()
{
	int newWidth = itemLabel.getFont().getStringWidth(itemLabel.getText())+ 15;
	if (item->isSelected) newWidth += removeBT->getWidth() + enabledBT->getWidth() + 10;
	setSize(newWidth, getHeight());
}

void TimeTriggerUI::mouseDown(const MouseEvent & e)
{
	BaseItemUI::mouseDown(e);
	timeAtMouseDown = item->time->floatValue();
	posAtMouseDown = getX();
	flagYAtMouseDown = item->flagY->floatValue();
}

void TimeTriggerUI::mouseDrag(const MouseEvent & e)
{
	if (itemLabel.isBeingEdited()) return;

	BaseItemUI::mouseDrag(e);

	if (!e.mods.isShiftDown())
	{
		triggerUIListeners.call(&TimeTriggerUIListener::timeTriggerDragged, this, e);
	}

	if (!e.mods.isCommandDown())
	{
		float ty = flagYAtMouseDown + e.getOffsetFromDragStart().y*1.f / (getHeight() - 20);
		item->flagY->setValue(ty);
	}

}

void TimeTriggerUI::mouseUp(const MouseEvent & e)
{
	BaseItemUI::mouseUp(e);

	if (flagYAtMouseDown == item->flagY->floatValue() && timeAtMouseDown == item->time->floatValue()) return;

	Array<UndoableAction *> actions;
	actions.add(item->flagY->setUndoableValue(flagYAtMouseDown, item->flagY->floatValue(), true));
	actions.add(item->time->setUndoableValue(timeAtMouseDown, item->time->floatValue(), true));
	UndoMaster::getInstance()->performActions("Move Trigger \""+item->niceName+"\"", actions);

}

void TimeTriggerUI::containerChildAddressChangedAsync(ControllableContainer * cc)
{
	BaseItemUI::containerChildAddressChangedAsync(cc);
	updateSizeFromName();
}

void TimeTriggerUI::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->time)
	{
		triggerUIListeners.call(&TimeTriggerUIListener::timeTriggerTimeChanged, this);
	} else if (c == item->flagY)
	{
		repaint();
		resized();
	} else if (c == item->isTriggered)
	{
		repaint();
	}
}

void TimeTriggerUI::inspectableSelectionChanged(Inspectable * i)
{
	BaseItemUI::inspectableSelectionChanged(i);
	removeBT->setVisible(item->isSelected);
	enabledBT->setVisible(item->isSelected);

	updateSizeFromName();
}
