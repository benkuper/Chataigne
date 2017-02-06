/*
  ==============================================================================

    Automation.cpp
    Created: 11 Dec 2016 1:21:37pm
    Author:  Ben

  ==============================================================================
*/

#include "Automation.h"
#include "AutomationEditor.h"

AutomationKeyComparator Automation::comparator;

Automation::Automation() :
	BaseManager("Automation"),
	positionMax(1),
	showUIInEditor(false)
{
	position = addFloatParameter("Position", "The current position in the automation. Used for automatic retrieve value and feedback.", 0, 0, positionMax);
	value = addFloatParameter("Value", "The current value, depending on the position", 0, 0, 1);
	value->isControllableFeedbackOnly = true;
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
	position->setRange(0, positionMax);
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

void Automation::addItem(const float _position, const float _value)
{
	AutomationKey * k = createItem();
	k->position->setValue(_position);
	k->value->setValue(_value);
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

void Automation::onContainerParameterChanged(Parameter * p)
{
	if (p == position)
	{
		value->setValue(getValueForPosition(position->floatValue()));
	}
}

InspectableEditor * Automation::getEditor(bool isRoot)
{
	return new AutomationEditor(this, isRoot);
}
