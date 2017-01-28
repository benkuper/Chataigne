/*
  ==============================================================================

    Automation.cpp
    Created: 11 Dec 2016 1:21:37pm
    Author:  Ben

  ==============================================================================
*/

#include "Automation.h"
#include "BaseItemEditor.h"

AutomationKeyComparator Automation::comparator;

Automation::Automation() :
	BaseManager("Automation"),
	positionMax(1)
{
	
}

Automation::~Automation()
{

}

void Automation::reorderItems()
{
	items.sort(Automation::comparator, true);
	BaseManager::reorderItems();
}

AutomationKey * Automation::getClosestKeyForPos(float pos, int start, int end)
{
	if (items.size() == 0) return nullptr;

	if (start == -1) start = 0;
	if (end == -1) end = items.size() - 1;


	if (pos < items[0]->position->floatValue()) return items[0];
	if (pos > items[items.size() - 1]->position->floatValue()) return items[items.size() - 1];


	if (end - start <= 1) return items[start];

	int midIndex = (int)floor((start + end) / 2);
	float medPos = items[midIndex]->position->floatValue();

	if (pos == medPos) return items[midIndex];

	else if (pos > medPos)
	{
		return getClosestKeyForPos(pos, midIndex, end);
	}
	else
	{
		return getClosestKeyForPos(pos, start, midIndex);
	}
}

void Automation::setPositionMax(float val)
{
	positionMax = val;
	for (auto &k : items) k->position->setRange(0, positionMax);
}

float Automation::getValueForPosition(float pos)
{
	if (items.size() == 0) return 0;
	if (pos <= items[0]->position->floatValue()) return items[0]->value->floatValue();
	else if (pos >= items[items.size() - 1]->position->floatValue()) return items[items.size() - 1]->value->floatValue();
	
	AutomationKey * k = getClosestKeyForPos(pos);
	if (k == nullptr) return 0;
	return k->getValue(items[items.indexOf(k) + 1], pos);
}

AutomationKey * Automation::createItem()
{
	AutomationKey * k = new AutomationKey();
	k->position->setRange(0, positionMax);
	return k;
}

void Automation::addItem(const float position, const float value)
{
	AutomationKey * k = createItem();
	k->position->setValue(position);
	k->value->setValue(value);
	BaseManager::addItem(k);
}

void Automation::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	AutomationKey * t = dynamic_cast<AutomationKey *>(cc);
	if (t != nullptr)
	{
		if (c == t->position)
		{
			int index = items.indexOf(t);
			if (index > 0 && t->position->floatValue() < items[index - 1]->position->floatValue())
			{
				items.swap(index, index - 1);
				baseManagerListeners.call(&Listener::itemsReordered);
			}
			else if (index < items.size() - 1 && t->position->floatValue() > items[index + 1]->position->floatValue())
			{
				items.swap(index, index + 1);
				baseManagerListeners.call(&Listener::itemsReordered);
			}
		}

	}
}

InspectableEditor * Automation::getEditor(bool isRoot)
{
	return new InspectableEditor(this, isRoot); //avoid having mess in editor when a lot of items
}
