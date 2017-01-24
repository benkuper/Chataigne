/*
  ==============================================================================

    ConditionManager.cpp
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionManager.h"
#include "ConditionManagerEditor.h"

juce_ImplementSingleton(ConditionManager)

ConditionManager::ConditionManager() :
	BaseManager<Condition>("Conditions")
{
	selectItemWhenCreated = false;

	isValid = addBoolParameter("Is Valid","Indicates if all the conditions are valid. If so, the consequences are triggered one time, at the moment the action becomes valid.",false);
	isValid->isSavable = false;
	isValid->isEditable = false;

}

ConditionManager::~ConditionManager()
{
}

void ConditionManager::addItemInternal(Condition * c, var data)
{
	c->addConditionListener(this);
}

void ConditionManager::removeItemInternal(Condition * c)
{
	c->removeConditionListener(this);
}

void ConditionManager::checkAllConditions()
{
	bool valid = areAllConditionsValid();
	if (valid != isValid->boolValue())
	{
		isValid->setValue(valid);
		conditionManagerListeners.call(&ConditionManagerListener::conditionManagerValidationChanged, this);
	}
}


void ConditionManager::conditionValidationChanged(Condition *)
{
	checkAllConditions();
}

InspectableEditor * ConditionManager::getEditor(bool isRoot)
{
	return new ConditionManagerEditor(this, isRoot);
}

bool ConditionManager::areAllConditionsValid()
{
	bool hasAtLeastOneValid = false;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->isValid->boolValue()) hasAtLeastOneValid = true;
		else return false;
	}

	return hasAtLeastOneValid;
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

int ConditionManager::getNumValidConditions()
{
	int result = 0;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->isValid->boolValue()) result++;
	}
	return result;
}
