/*
  ==============================================================================

    Automation.cpp
    Created: 11 Dec 2016 1:21:37pm
    Author:  Ben

  ==============================================================================
*/

#include "Automation.h"

AutomationKeyComparator Automation::comparator;

Automation::Automation() :
	BaseManager("Automation"),
	valueMax(1),
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

	if (end == start) return items[start];

	int midIndex = (start + end) / 2;
	float medPos = items[midIndex]->position->floatValue();

	if (pos == medPos) return items[midIndex];
	else if (pos > medPos)
	{
		return getClosestKeyForPos(medPos + 1, end);
	}
	else
	{
		return getClosestKeyForPos(start, medPos);
	}
}

void Automation::setPositionMax(float val)
{
	positionMax = val;
	for (auto &k : items) k->position->setRange(0, positionMax);
}

void Automation::addItem(const float position, const float value)
{
	AutomationKey * k = new AutomationKey();
	k->position->setRange(0, positionMax);
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
