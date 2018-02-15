/*
  ==============================================================================

	AutomationMultiKeyTransformer.cpp
	Created: 25 Mar 2017 4:14:45pm
	Author:  Ben

  ==============================================================================
*/

#include "AutomationMultiKeyTransformer.h"
#include "AutomationUI.h"

AutomationMultiKeyTransformer::AutomationMultiKeyTransformer(AutomationUI * _aui, Array<AutomationKeyUI*> _keysUI) :
	aui(_aui),
	keysUI(_keysUI),
	resizer(this, nullptr)
{

	resizer.setBorderThickness(BorderSize<int>(6, 6, 6, 6));
	addAndMakeVisible(&resizer);

	updateBoundsFromKeys();

}

AutomationMultiKeyTransformer::~AutomationMultiKeyTransformer()
{
	for (auto &k : keysUI) k->setEnabled(true);

}

void AutomationMultiKeyTransformer::updateBoundsFromKeys()
{

	Point<int> b0 = aui->getLocalPoint(&keysUI[0]->handle, keysUI[0]->handle.getLocalBounds().getCentre());
	keyBounds.setPosition(b0.getX(), b0.getY());
	keyBounds.setSize(0, 0);

	for (auto &k : keysUI)
	{
		Point<int> b = aui->getLocalPoint(&k->handle, k->handle.getLocalBounds().getCentre());
		keyBounds.setLeft(jmin(b.getX(), keyBounds.getX()));
		keyBounds.setRight(jmax(b.getX(), keyBounds.getRight()));
		keyBounds.setTop(jmin(b.getY(), keyBounds.getY()));
		keyBounds.setBottom(jmax(b.getY(), keyBounds.getBottom()));
	}

	setBounds(keyBounds);
}

void AutomationMultiKeyTransformer::updateKeysFromBounds()
{
	int numKeys = keysUI.size();
	for (int i = 0; i < numKeys; i++)
	{
		Point<int> localPos = getLocalBounds().getRelativePoint(keysRelativePositions[i].x, keysRelativePositions[i].y);
		Point<int> timelinePos = aui->getLocalPoint(this, localPos);
		float targetPos = aui->getPosForX(timelinePos.x);
		float targetVal = 1 - (timelinePos.y*1.f / aui->getHeight());
		keysUI[i]->item->position->setValue(targetPos);
		keysUI[i]->item->value->setValue(targetVal);
	}
}

void AutomationMultiKeyTransformer::parentHierarchyChanged()
{
	if (getParentComponent() == nullptr) return;

	if (keysRelativePositions.size() == 0)
	{
		for (auto &k : keysUI)
		{
			k->setEnabled(false);
			keysRelativePositions.add(getLocalPoint(&k->handle, k->handle.getLocalBounds().getCentre()).toFloat() / Point<float>(getWidth(), getHeight()));
		}
	}
}

void AutomationMultiKeyTransformer::resized()
{

	if (resizer.isMouseOverOrDragging()) updateKeysFromBounds();
	resizer.setBounds(getLocalBounds().reduced(1));
}

void AutomationMultiKeyTransformer::paint(Graphics & g)
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

void AutomationMultiKeyTransformer::mouseDown(const MouseEvent & e)
{
	posAtMouseDown = getBounds().getPosition();
	keysTimesAndValuesPositions.clear();
	for (auto &k : keysUI)
	{
		keysTimesAndValuesPositions.add(Point<float>(k->item->position->floatValue(), k->item->value->floatValue()));
	}
}

void AutomationMultiKeyTransformer::mouseDrag(const MouseEvent & e)
{
	setTopLeftPosition(posAtMouseDown + e.getOffsetFromDragStart());
	updateKeysFromBounds();
}

void AutomationMultiKeyTransformer::mouseUp(const MouseEvent & e)
{
	if (e.getOffsetFromDragStart().getDistanceFromOrigin() == 0) return;

	Array<UndoableAction *> actions;
	int numKeys = keysUI.size();
	for (int i = 0; i < numKeys; i++)
	{
		actions.add(keysUI[i]->item->position->setUndoableValue(keysTimesAndValuesPositions[i].x, keysUI[i]->item->position->floatValue(), true));
		actions.add(keysUI[i]->item->value->setUndoableValue(keysTimesAndValuesPositions[i].y, keysUI[i]->item->value->floatValue(), true));
	}
	UndoMaster::getInstance()->performActions("Move " + String(numKeys) + " keys", actions);
}
