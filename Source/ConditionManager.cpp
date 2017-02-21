/*
  ==============================================================================

    ConditionManager.cpp
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionManager.h"
#include "ConditionManagerEditor.h"
#include "ConditionFactory.h"

juce_ImplementSingleton(ConditionManager)

ConditionManager::ConditionManager(bool _operatorOnSide) :
	BaseManager<Condition>("Conditions"),
	operatorOnSide(_operatorOnSide)
{
	selectItemWhenCreated = false;

	isValid = addBoolParameter("Is Valid","Indicates if all the conditions are valid. If so, the consequences are triggered one time, at the moment the action becomes valid.",false);
	isValid->isControllableFeedbackOnly = true;
	isValid->hideInEditor = true;

	conditionOperator = addEnumParameter("Operator", "Operator for this manager, will decides how the conditions are validated");
	conditionOperator->addOption("AND", ConditionOperator::AND);
	conditionOperator->addOption("OR", ConditionOperator::OR);
}

ConditionManager::~ConditionManager()
{
}

void ConditionManager::addItemFromData(var data)
{

	String conditionType = data.getProperty("type", "none");
	if (conditionType.isEmpty()) return;
	Condition * i = ConditionFactory::getInstance()->createModule(conditionType);
	if (i != nullptr) addItem(i, data);
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
	bool valid = false;
	ConditionOperator op = (ConditionOperator)(int)conditionOperator->getValueData();
	switch (op)
	{
	case ConditionOperator::AND:
		valid = areAllConditionsValid();
		break;

	case ConditionOperator::OR:
		valid = isAtLeastOneConditionValid();
		break;
	}
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
	if (items.size() == 0) return false;

	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (!c->isValid->boolValue()) return false;
	}

	return true;
}

bool ConditionManager::isAtLeastOneConditionValid()
{
	if (items.size() == 0) return false;

	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->isValid->boolValue()) return true;
	}

	return false;
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

bool ConditionManager::getIsValid(bool emptyIsValid)
{
	return isValid->boolValue() || (emptyIsValid && items.size() == 0);
}
