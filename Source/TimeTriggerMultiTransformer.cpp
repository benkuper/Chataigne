/*
  ==============================================================================

    TimeTriggerMultiTransformer.cpp
    Created: 18 Jan 2018 5:33:20pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeTriggerMultiTransformer.h"
#include "TimeTriggerManagerUI.h"
#include "TriggerLayerTimeline.h"

TimeTriggerMultiTransformer::TimeTriggerMultiTransformer(TimeTriggerManagerUI * _tui, Array<TimeTriggerUI*> _triggersUI) :
	tui(_tui),
	triggersUI(_triggersUI),
	resizer(this, nullptr)
{

	resizer.setBorderThickness(BorderSize<int>(6, 6, 6, 6));
	addAndMakeVisible(&resizer);

	updateBoundsFromKeys();

	setInterceptsMouseClicks(true, true);
	resizer.addMouseListener(this, true);
}

TimeTriggerMultiTransformer::~TimeTriggerMultiTransformer()
{
	for (auto &k : triggersUI) k->setEnabled(true);
}

void TimeTriggerMultiTransformer::updateBoundsFromKeys()
{
	Point<int> b0 = tui->getLocalPoint(triggersUI[0], triggersUI[0]->flagRect.getTopLeft());
	keyBounds.setPosition(b0.getX(), b0.getY());
	keyBounds.setSize(0, 0);

	for (auto &k : triggersUI)
	{
		keyBounds.setLeft(jmin(k->getX(), keyBounds.getX()));
		keyBounds.setRight(jmax(k->getRight(), keyBounds.getRight()));
		keyBounds.setTop(jmin(k->getY()+k->flagRect.getY(), keyBounds.getY()));
		keyBounds.setBottom(jmax(k->getY()+k->flagRect.getBottom(), keyBounds.getBottom()));
	}

	setBounds(keyBounds);
}

void TimeTriggerMultiTransformer::updateKeysFromBounds()
{
	int numKeys = triggersUI.size();
	for (int i = 0; i < numKeys; i++)
	{
		Point<int> localPos = getLocalBounds().getRelativePoint(triggersRelativePositions[i].x, triggersRelativePositions[i].y);
		Point<int> timelinePos = tui->getLocalPoint(this, localPos);
		float targetTime = tui->timeline->getTimeForX(timelinePos.x);
		float targetFlagY = timelinePos.y*1.f / tui->getHeight();
		triggersUI[i]->item->time->setValue(targetTime);
		triggersUI[i]->item->flagY->setValue(targetFlagY);
	}
}

void TimeTriggerMultiTransformer::parentHierarchyChanged()
{
	if (getParentComponent() == nullptr) return;

	if (triggersRelativePositions.size() == 0)
	{
		for (auto &k : triggersUI)
		{
			k->setEnabled(false);
			triggersRelativePositions.add(getLocalPoint(k,Point<float>(0,k->item->flagY->floatValue()*tui->getHeight())).toFloat() / Point<float>(getWidth(), getHeight()));
		}
	}
}

void TimeTriggerMultiTransformer::resized()
{
	if (resizer.isMouseOverOrDragging()) updateKeysFromBounds();
	resizer.setBounds(getLocalBounds().reduced(1));
}

void TimeTriggerMultiTransformer::paint(Graphics & g)
{
	g.setColour(Colours::white.withAlpha(.1f));
	g.fillRect(getLocalBounds());


	Rectangle<int> r = getLocalBounds();
	Line<float> l1(r.getX(), r.getY(), r.getRight(), r.getY());
	Line<float> l2(r.getRight(), r.getY(), r.getRight(), r.getBottom());
	Line<float> l3(r.getRight(), r.getBottom(), r.getX(), r.getBottom());
	Line<float> l4(r.getX(), r.getBottom(), r.getX(), r.getY());
	float dl[] = { 4,2 };

	g.setColour(Colours::white);
	g.drawDashedLine(l1, dl, 2, 1);
	g.drawDashedLine(l2, dl, 2, 1);
	g.drawDashedLine(l3, dl, 2, 1);
	g.drawDashedLine(l4, dl, 2, 1);
}

void TimeTriggerMultiTransformer::mouseDown(const MouseEvent & e)
{ 
	posAtMouseDown = getBounds().getPosition();
	triggersTimesAndValuesPositions.clear();
	for (auto &k : triggersUI)
	{
		triggersTimesAndValuesPositions.add(Point<float>(k->item->time->floatValue(), k->item->flagY->floatValue()));
	}
}

void TimeTriggerMultiTransformer::mouseDrag(const MouseEvent & e)
{
	if (e.eventComponent != this) return;
	setTopLeftPosition(posAtMouseDown + e.getOffsetFromDragStart());
	updateKeysFromBounds();
}

void TimeTriggerMultiTransformer::mouseUp(const MouseEvent & e)
{
	if (e.getOffsetFromDragStart().getDistanceFromOrigin() == 0) return;

	Array<UndoableAction *> actions;
	int numKeys = triggersUI.size();
	for (int i = 0; i < numKeys; i++)
	{
		actions.add(triggersUI[i]->item->time->setUndoableValue(triggersTimesAndValuesPositions[i].x, triggersUI[i]->item->time->floatValue(), true));
		actions.add(triggersUI[i]->item->flagY->setUndoableValue(triggersTimesAndValuesPositions[i].y, triggersUI[i]->item->flagY->floatValue(), true));
	}

	UndoMaster::getInstance()->performActions("Move " + String(numKeys) + " keys", actions);
}

