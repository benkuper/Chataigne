/*
  ==============================================================================

    TimeTriggerManager.cpp
    Created: 10 Dec 2016 12:22:48pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeTriggerManager.h"
#include "Sequence.h"

TimeTriggerComparator TimeTriggerManager::comparator;

TimeTriggerManager::TimeTriggerManager(Sequence * _sequence) :
	BaseManager<TimeTrigger>("Triggers"),
	sequence(_sequence)
{
	skipControllableNameInAddress = true;

	sequence->addSequenceListener(this);
}

TimeTriggerManager::~TimeTriggerManager()
{
	sequence->removeSequenceListener(this);
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

Array<TimeTrigger*> TimeTriggerManager::getTriggersInTimespan(float startTime, float endTime, bool includeAlreadyTriggered)
{
	Array<TimeTrigger*> result;
	for (auto &tt : items)
	{
		if (tt->time->floatValue() >= startTime && tt->time->floatValue() <= endTime && (includeAlreadyTriggered || !tt->isTriggered->boolValue()))
		{
			result.add(tt);
		}
	}
	return result;
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

void TimeTriggerManager::sequenceCurrentTimeChanged(Sequence * /*_sequence*/, float prevTime, bool evaluateSkippedData)
{

	if (sequence->currentTime->floatValue() >= prevTime)
	{ 
		if (evaluateSkippedData || ModifierKeys::getCurrentModifiers().isCtrlDown())
		{
			Array<TimeTrigger *> spanTriggers = getTriggersInTimespan(prevTime, sequence->currentTime->floatValue());
			for (auto &tt : spanTriggers)
			{
				DBG("here ! ");
				tt->trigger->trigger();
			}
		}
	}else //loop or manual, untrigger
	{
		Array<TimeTrigger *> spanTriggers = getTriggersInTimespan(sequence->currentTime->floatValue(),prevTime,true);
		for (auto &tt : spanTriggers)
		{
			tt->isTriggered->setValue(false);
		}
	}
	
	
}
