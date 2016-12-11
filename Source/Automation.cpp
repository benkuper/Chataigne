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

void Automation::addItemInternal(AutomationKey *, var data)
{
	reorderKeys();
}

void Automation::reorderKeys()
{
	items.sort(Automation::comparator, true);
	baseManagerListeners.call(&Listener::itemsReordered);
}

AutomationKey * Automation::addItem(const float position, const float value)
{
	AutomationKey * k = BaseManager::addItem();
	k->position->setValue(position);
	k->value->setValue(value);
	return k;
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
