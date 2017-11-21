/*
  ==============================================================================

	TimeCueManager.cpp
	Created: 6 Apr 2017 11:13:27am
	Author:  Ben

  ==============================================================================
*/

#include "TimeCueManager.h"

TimeCueComparator TimeCueManager::comparator;

TimeCueManager::TimeCueManager() :
	BaseManager("Cues")
{
}

TimeCueManager::~TimeCueManager()
{

}

void TimeCueManager::addCueAt(float time)
{
	TimeCue * t = new TimeCue(time);
	BaseManager::addItem(t);
}

void TimeCueManager::reorderItems()
{
	items.sort(TimeCueManager::comparator, true);
	BaseManager::reorderItems();
}

Array<float> TimeCueManager::getAllCueTimes(float minTime, float maxTime)
{
	Array<float> result;
	for (auto &c : items)
	{
		float t = c->time->floatValue();
		if (maxTime > 0 && (t < minTime || t > maxTime)) continue;
		result.add(t);
	}
	return result;
}

float TimeCueManager::getNearestCueForTime(float time)
{
	float result = time;
	if (items.size() == 0) return result;
    float diffTime = std::abs(time - items[0]->time->floatValue());
	result = items[0]->time->floatValue();
	int numItems = items.size();

	for (int i = 1; i < numItems; i++)
	{
		float newTime = items[i]->time->floatValue();
		float newDiff = std::abs(time - newTime);
		if (newDiff > diffTime) break;
		diffTime = newDiff;
		result = newTime;
	}
	return result;
}

float TimeCueManager::getNextCueForTime(float time)
{
	int numItems = items.size();
	float result = time;
	for (int i = numItems - 1; i >= 0; i--)
	{
		float t = items[i]->time->floatValue();
		if (t <= time) break;
		result = t;
	}

	return result;
}

float TimeCueManager::getPrevCueForTime(float time, float goToPreviousThreshold)
{
	int numItems = items.size();
	float result = time;
	for (int i = 0; i < numItems; i++)
	{
		float t = items[i]->time->floatValue();
		if (t > time-goToPreviousThreshold) break;
		result = t;
	}

	return result;
}

void TimeCueManager::onControllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	TimeCue * t = static_cast<TimeCue *>(cc);
	if (t != nullptr)
	{
		if (c == t->time)
		{
			int index = items.indexOf(t);
			if (index > 0 && t->time->floatValue() < items[index - 1]->time->floatValue())
			{
				items.swap(index, index - 1);
				baseManagerListeners.call(&Listener::itemsReordered);
			}
			else if (index < items.size() - 1 && t->time->floatValue() > items[index + 1]->time->floatValue())
			{
				items.swap(index, index + 1);
				baseManagerListeners.call(&Listener::itemsReordered);
			}
		}

	}
}
