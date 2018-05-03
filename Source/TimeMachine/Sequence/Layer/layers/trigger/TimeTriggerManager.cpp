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
	itemDataType = "TimeTrigger";
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

TimeTrigger * TimeTriggerManager::addItemFromClipboard(bool showWarning)
{
	TimeTrigger * t = BaseManager::addItemFromClipboard(showWarning);
	if (t == nullptr) return nullptr;
	if(sequence->currentTime->floatValue() > 0) t->time->setValue(sequence->currentTime->floatValue()); //only set time if time is > 0, most copy happen when sequence has not been launched
	return t;
}

bool TimeTriggerManager::canAddItemOfType(const String & typeToCheck)
{
	return typeToCheck == itemDataType || typeToCheck == "Action";
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
				tt->trigger();
			}
		}
	}else //loop or manual, untrigger
	{
		Array<TimeTrigger *> spanTriggers = getTriggersInTimespan(sequence->currentTime->floatValue(),sequence->totalTime->floatValue(),true);
		for (auto &tt : spanTriggers)
		{
			tt->isTriggered->setValue(false);
		}
	}
	
	
}
