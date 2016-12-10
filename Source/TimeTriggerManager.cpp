/*
  ==============================================================================

    TimeTriggerManager.cpp
    Created: 10 Dec 2016 12:22:48pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeTriggerManager.h"

TimeTriggerComparator TimeTriggerManager::comparator;

TimeTriggerManager::TimeTriggerManager() :
	BaseManager<TimeTrigger>("Triggers")
{
}

TimeTriggerManager::~TimeTriggerManager()
{
}




void TimeTriggerManager::addItemInternal(TimeTrigger * , var )
{
	reorderTriggers();
}

void TimeTriggerManager::removeItemInternal(TimeTrigger *)
{
	//reorderTriggers();
}


void TimeTriggerManager::reorderTriggers()
{
	items.sort(TimeTriggerManager::comparator, true);
	baseManagerListeners.call(&Listener::itemsReordered);
}

void TimeTriggerManager::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	
	TimeTrigger * t = dynamic_cast<TimeTrigger *>(cc);
	if (t != nullptr)
	{
		if (c == t->time)
		{
			int index = items.indexOf(t);
			if (index > 0 && t->time->floatValue() < items[index - 1]->time->floatValue())
			{
				items.swap(index, index - 1);
				baseManagerListeners.call(&Listener::itemsReordered);
			}else if(index < items.size()-1 && t->time->floatValue() > items[index + 1]->time->floatValue()) 
			{
				items.swap(index, index + 1);
				baseManagerListeners.call(&Listener::itemsReordered);
			}
		}

	}
}
