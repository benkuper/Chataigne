/*
  ==============================================================================

    ConditionManager.cpp
    Created: 28 Oct 2016 8:07:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "ConditionManager.h"
#include "ui/ConditionManagerEditor.h"
#include "conditions/StandardCondition/StandardCondition.h"
#include "conditions/ConditionGroup/ConditionGroup.h"
#include "conditions/ScriptCondition/ScriptCondition.h"

juce_ImplementSingleton(ConditionManager)

ConditionManager::ConditionManager(bool _operatorOnSide) :
	BaseManager<Condition>("Conditions"),
	operatorOnSide(_operatorOnSide),
	validationProgress(nullptr),
	forceDisabled(false)
{
	
	managerFactory = &factory;
	factory.defs.add(Factory<Condition>::Definition::createDef("", StandardCondition::getTypeStringStatic(), &StandardCondition::create));
	factory.defs.add(Factory<Condition>::Definition::createDef("", ConditionGroup::getTypeStringStatic(), &ConditionGroup::create));
	factory.defs.add(Factory<Condition>::Definition::createDef("", ScriptCondition::getTypeStringStatic(), &ScriptCondition::create));

	selectItemWhenCreated = false;

	isValid = addBoolParameter("Is Valid","Indicates if all the conditions are valid. If so, the consequences are triggered one time, at the moment the action becomes valid.",false);
	isValid->isControllableFeedbackOnly = true;
	isValid->hideInEditor = true;

	conditionOperator = addEnumParameter("Operator", "Operator for this manager, will decides how the conditions are validated");
	conditionOperator->addOption("AND", ConditionOperator::AND);
	conditionOperator->addOption("OR", ConditionOperator::OR);
	conditionOperator->hideInEditor = true;

	validationTime = addFloatParameter("Validation Time", "If greater than 0, the conditions will be validated only if they remain valid for this amount of time", 0, 0, (float)INT32_MAX);
	validationTime->hideInEditor = true;
	validationTime->defaultUI = FloatParameter::TIME;

	validationProgress = addFloatParameter("Progress", "Validation time progress", 0, 0, validationTime->floatValue());
	validationProgress->isControllableFeedbackOnly = true;
	validationProgress->isEditable = false;
	validationProgress->setEnabled(false);
	validationProgress->hideInEditor = true;
}

ConditionManager::~ConditionManager()
{
}


void ConditionManager::addItemInternal(Condition * c, var data)
{
	c->addConditionListener(this);
	conditionOperator->hideInEditor = items.size() <= 1;
	validationTime->hideInEditor = items.size() == 0;
	validationProgress->hideInEditor = items.size() == 0;
	checkAllConditions();
}

void ConditionManager::removeItemInternal(Condition * c)
{
	c->removeConditionListener(this);
	conditionOperator->hideInEditor = items.size() <= 1;
	validationTime->hideInEditor = items.size() == 0;
	validationProgress->hideInEditor = items.size() == 0;
	checkAllConditions();
}

void ConditionManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	if (forceDisabled) isValid->setValue(false);
	for (auto &i : items) i->forceDisabled = value;
}

void ConditionManager::checkAllConditions(bool emptyIsValid)
{
	bool valid = false;
	ConditionOperator op = (ConditionOperator)(int)conditionOperator->getValueData();
	switch (op)
	{
	case ConditionOperator::AND:
		valid = areAllConditionsValid(emptyIsValid);
		break;

	case ConditionOperator::OR:
		valid = isAtLeastOneConditionValid(emptyIsValid);
		break;
	}

	if (validationTime->floatValue() == 0)
	{
		if (valid != isValid->boolValue())
		{
			isValid->setValue(valid);
			dispatchConditionValidationChanged();
		}
	}else if (valid != validationWaiting)
	{
		validationWaiting = valid;

		validationProgress->setValue(0);
		isValid->setValue(false);

		if(!valid)
		{
			stopTimer();
			dispatchConditionValidationChanged();
		} else
		{

			prevTimerTime = Time::getHighResolutionTicks();
			startTimer(20);
		}
	}
}


void ConditionManager::conditionValidationChanged(Condition *)
{
	checkAllConditions();
}

void ConditionManager::onContainerParameterChanged(Parameter * p)
{
	if (p == validationTime)
	{
		validationProgress->setEnabled(validationTime->floatValue() > 0);
		validationProgress->setRange(0, validationTime->floatValue());
	} else if (p == conditionOperator)
	{
		checkAllConditions();
	}
}


InspectableEditor * ConditionManager::getEditor(bool isRoot)
{
	return new ConditionManagerEditor(this, isRoot);
}

void ConditionManager::timerCallback()
{
	if (validationTime->floatValue() == 0)
	{
		isValid->setValue(true);
		dispatchConditionValidationChanged();
		stopTimer();
		return;
	}

	double curTime = Time::getMillisecondCounterHiRes();
	validationProgress->setValue(validationProgress->floatValue() + (curTime - prevTimerTime)/1000.f);
	prevTimerTime = curTime;


	if (validationProgress->floatValue() >= validationTime->floatValue())
	{
		isValid->setValue(true);
		dispatchConditionValidationChanged();
		stopTimer();
	}
}

bool ConditionManager::areAllConditionsValid(bool emptyIsValid)
{
	if (items.size() == 0) return emptyIsValid;

	int conditionsChecked = 0;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (!c->isValid->boolValue()) return false;
		conditionsChecked++;
	}

	if (conditionsChecked == 0) return emptyIsValid;
	return true;
}

bool ConditionManager::isAtLeastOneConditionValid(bool emptyIsValid)
{
	if (items.size() == 0) return emptyIsValid;

	int conditionsChecked = 0; 
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->isValid->boolValue()) return true;
		conditionsChecked++;
	}

	if (conditionsChecked == 0) return emptyIsValid;
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

void ConditionManager::dispatchConditionValidationChanged()
{
	conditionManagerListeners.call(&ConditionManagerListener::conditionManagerValidationChanged, this);
}
