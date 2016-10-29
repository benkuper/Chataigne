/*
  ==============================================================================

    ConditionManager.cpp
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionManager.h"

juce_ImplementSingleton(ConditionManager)

ConditionManager::ConditionManager() :
	BaseManager<Condition>("Conditions")
{
}

ConditionManager::~ConditionManager()
{
}

void ConditionManager::conditionEnableChanged(Condition *)
{
}

void ConditionManager::conditionValidationChanged(Condition *)
{
}

void ConditionManager::conditionActivationChanged(Condition *)
{
}

bool ConditionManager::areAllConditionActive()
{
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (!c->isActive->boolValue()) return false;
	}

	return true;
}

int ConditionManager::getNumEnabledConditions()
{
	int result = 0;
	for (auto &c : items)
	{
		if (c->enabled->boolValue()) result++;
	}
	return result;
}

int ConditionManager::getNumActivatedConditions()
{
	int result = 0;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->isActive->boolValue()) result++;
	}
	return result;
}
