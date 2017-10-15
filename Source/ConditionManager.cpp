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
	operatorOnSide(_operatorOnSide),
	validationProgress(nullptr)
{
	selectItemWhenCreated = false;

	isValid = addBoolParameter("Is Valid","Indicates if all the conditions are valid. If so, the consequences are triggered one time, at the moment the action becomes valid.",false);
	isValid->isControllableFeedbackOnly = true;
	isValid->hideInEditor = true;

	conditionOperator = addEnumParameter("Operator", "Operator for this manager, will decides how the conditions are validated");
	conditionOperator->addOption("AND", ConditionOperator::AND);
	conditionOperator->addOption("OR", ConditionOperator::OR);
	conditionOperator->hideInEditor = true;

	validationTime = addFloatParameter("Validation Time", "If greater than 0, the conditions will be validated only if they remain valid for this amount of time", 0, 0, 10);
	
	//conditionOperator->hideInEditor = items.size() <= 1;
}

ConditionManager::~ConditionManager()
{
}

void ConditionManager::addItemFromData(var data, bool fromUndoableAction)
{

	String conditionType = data.getProperty("type", "none");
	if (conditionType.isEmpty()) return;
	Condition * i = ConditionFactory::getInstance()->createModule(conditionType);
	if (i != nullptr) addItem(i, data, fromUndoableAction);
}

void ConditionManager::addItemInternal(Condition * c, var data)
{
	c->addConditionListener(this);
	conditionOperator->hideInEditor = items.size() <= 1;
}

void ConditionManager::removeItemInternal(Condition * c)
{
	c->removeConditionListener(this);
	conditionOperator->hideInEditor = items.size() <= 1;
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


	
	DBG("Check all conditions ");

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

			prevTimerTime = Time::getMillisecondCounter() / 1000.0f;
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
		if (validationTime->floatValue() > 0)
		{
			if (validationProgress == nullptr)
			{
				validationProgress = addFloatParameter("Progress", "Validation time progress", 0, 0, validationTime->floatValue());
				validationProgress->isControllableFeedbackOnly = true;
				validationProgress->isEditable = false;

			}
			else validationProgress->setRange(0, validationTime->floatValue());
		} else
		{
			if (validationProgress != nullptr)
			{
				removeControllable(validationProgress);
				validationProgress = nullptr;
			}
		}
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
	if (validationProgress == nullptr)
	{
		isValid->setValue(true);
		dispatchConditionValidationChanged();
		stopTimer();
		return;
	}

	double curTime = Time::getMillisecondCounter() / 1000.0f;
	validationProgress->setValue(validationProgress->floatValue() + (curTime - prevTimerTime));
	prevTimerTime = curTime;


	if (validationProgress->floatValue() >= validationTime->floatValue())
	{
		isValid->setValue(true);
		dispatchConditionValidationChanged();
		stopTimer();
	}
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

void ConditionManager::dispatchConditionValidationChanged()
{
	conditionManagerListeners.call(&ConditionManagerListener::conditionManagerValidationChanged, this);
}
