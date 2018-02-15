/*
  ==============================================================================

    TimeTriggerManager.cpp
    Created: 10 Dec 2016 12:22:48pm
    Author:  Ben

  ==============================================================================
*/

#include "TimeTriggerManager.h"
#include "../../../Sequence.h"
#include "TriggerLayer.h"

TimeTriggerComparator TimeTriggerManager::comparator;

TimeTriggerManager::TimeTriggerManager(TriggerLayer * _layer, Sequence * _sequence) :
	BaseManager<TimeTrigger>("Triggers"),
	layer(_layer),
	sequence(_sequence)
{
	skipControllableNameInAddress = true;
	sequence->addSequenceListener(this);
}

TimeTriggerManager::~TimeTriggerManager()
{
	sequence->removeSequenceListener(this);
}


void TimeTriggerManager::addTriggerAt(float time,float flagY)
{
	TimeTrigger * t = new TimeTrigger(time,flagY);
	BaseManager::addItem(t);
}

void TimeTriggerManager::reorderItems()
{
	items.sort(TimeTriggerManager::comparator, true);
	BaseManager::reorderItems();
}

TimeTrigger * TimeTriggerManager::addItemFromClipboard()
{
	TimeTrigger * t = BaseManager::addItemFromClipboard();
	if (t == nullptr) return nullptr;
	t->time->setValue(sequence->currentTime->floatValue());
	return t;
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

void TimeTriggerManager::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	TimeTrigger * t = static_cast<TimeTrigger *>(cc);
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
	if (!layer->enabled->boolValue() || !sequence->enabled->boolValue()) return;

	if (sequence->currentTime->floatValue() >= prevTime)
	{ 
		if (evaluateSkippedData || ModifierKeys::getCurrentModifiers().isCtrlDown())
		{
			Array<TimeTrigger *> spanTriggers = getTriggersInTimespan(prevTime, sequence->currentTime->floatValue());
			for (auto &tt : spanTriggers)
			{
				tt->triggerOn->trigger();
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
