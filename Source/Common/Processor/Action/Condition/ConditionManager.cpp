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
#include "conditions/ActivationCondition/ActivationCondition.h"
#include "Common/Processor/Action/Action.h"

ConditionManager::ConditionManager(Multiplex * multiplex) :
	MultiplexTarget(multiplex),
	BaseManager<Condition>("Conditions"),
	activateDef(nullptr),
	deactivateDef(nullptr),
    forceDisabled(false),
	conditionManagerAsyncNotifier(10)
{
	canBeCopiedAndPasted = true;
	selectItemWhenCreated = false;

	managerFactory = &factory;
	factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<StandardCondition>("", StandardCondition::getTypeStringStatic(false), multiplex));
	if (isMultiplexed()) factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<StandardCondition>("", StandardCondition::getTypeStringStatic(true), multiplex)->addParam("listMode", true));

	factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<ConditionGroup>("", ConditionGroup::getTypeStringStatic(), multiplex));
	factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<ScriptCondition>("", ScriptCondition::getTypeStringStatic(), multiplex));
	
	validationTime = addFloatParameter("Validation Time", "If greater than 0, the conditions will be validated only if they remain valid for this amount of time", 0, 0, (float)INT32_MAX);
	validationTime->defaultUI = FloatParameter::TIME;
	validationProgressFeedback = addFloatParameter("Validation Progress", "The feedback of the progress if validation time is more than 0", 0, 0, 1, false);
	validationProgressFeedback->setControllableFeedbackOnly(true);
	
	conditionOperator = addEnumParameter("Operator", "Operator for this manager, will decides how the conditions are validated");
	conditionOperator->addOption("AND", ConditionOperator::AND);
	conditionOperator->addOption("OR", ConditionOperator::OR);
	conditionOperator->hideInEditor = true;
}

ConditionManager::~ConditionManager()
{
}

void ConditionManager::multiplexCountChanged()
{
	isValids.resize(getMultiplexCount());
	validationProgresses.resize(getMultiplexCount());

	isValids.fill(false);
	validationProgresses.fill(0);
}



void ConditionManager::setHasActivationDefinitions(bool value)
{
	if (value)
	{
		if (activateDef == nullptr && deactivateDef == nullptr)
		{
			activateDef = (Factory<Condition>::Definition *)Factory<Condition>::Definition::createDef<ActivationCondition>("", ActivationCondition::getTypeStringStatic(ActivationCondition::ON_ACTIVATE))->addParam("type", ActivationCondition::ON_ACTIVATE);
			deactivateDef = (Factory<Condition>::Definition *)Factory<Condition>::Definition::createDef<ActivationCondition>("", ActivationCondition::getTypeStringStatic(ActivationCondition::ON_DEACTIVATE))->addParam("type", ActivationCondition::ON_DEACTIVATE);
			factory.defs.add(activateDef);
			factory.defs.add(deactivateDef);
			factory.buildPopupMenu();
		}
	} else
	{
		factory.defs.removeObject(activateDef);
		factory.defs.removeObject(deactivateDef);
		activateDef = nullptr;
		deactivateDef = nullptr;
		factory.buildPopupMenu();
	}
}

void ConditionManager::addItemInternal(Condition * c, var data)
{
	c->setForceDisabled(forceDisabled);
	c->addConditionListener(this);
	conditionOperator->hideInEditor = items.size() <= 1;
	StandardCondition* sc = dynamic_cast<StandardCondition*>(c);
	if (sc != nullptr)
	{
		Action* a = ControllableUtil::findParentAs<Action>(this);
		sc->sourceTarget->warningResolveInspectable = a != nullptr ? (Inspectable*)a : this;
	}
}

void ConditionManager::removeItemInternal(Condition * c)
{
	c->removeConditionListener(this);
	conditionOperator->hideInEditor = items.size() <= 1;
	if (!Engine::mainEngine->isLoadingFile && !Engine::mainEngine->isClearing)
	{
		for (int i = 0; i < getMultiplexCount(); i++) checkAllConditions(i);
	}
}

void ConditionManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return; 
	forceDisabled = value;
	if (forceDisabled)
	{
		isValids.fill(false);
	}

	for (auto &i : items) i->setForceDisabled(value);

	for (int i = 0; i < getMultiplexCount(); i++) checkAllConditions(i);
}

void ConditionManager::setValid(int multiplexIndex, bool value, bool dispatchOnlyOnValidationChange)
{
	if (isValids[multiplexIndex] == value && dispatchOnlyOnValidationChange) return;
	isValids.set(multiplexIndex, value);
	dispatchConditionValidationChanged(multiplexIndex);
}

void ConditionManager::setValidationProgress(int multiplexIndex, float value)
{
	validationProgresses.set(multiplexIndex, value);
	if (!isMultiplexed()) validationProgressFeedback->setValue(value);
}

void ConditionManager::forceCheck()
{
	for (auto& i : items) i->forceCheck();
}

void ConditionManager::checkAllConditions(int multiplexIndex, bool emptyIsValid, bool dispatchOnlyOnValidationChange)
{
	bool valid = false;
	ConditionOperator op = (ConditionOperator)(int)conditionOperator->getValueData();
	switch (op)
	{
	case ConditionOperator::AND:
		valid = areAllConditionsValid(multiplexIndex, emptyIsValid);
		break;

	case ConditionOperator::OR:
		valid = isAtLeastOneConditionValid(multiplexIndex, emptyIsValid);
		break;
	}

	if (validationTime->floatValue() == 0)
	{
		setValid(multiplexIndex, valid, dispatchOnlyOnValidationChange);
	}else if (valid != validationWaitings[multiplexIndex])
	{
		validationWaitings.set(multiplexIndex, valid);
		setValidationProgress(multiplexIndex, 0);
		setValid(multiplexIndex, false);
		if(!valid)
		{
			stopTimer(multiplexIndex);
		} else
		{
			prevTimerTimes.set(multiplexIndex, Time::getMillisecondCounterHiRes() / 1000.0);
			startTimer(multiplexIndex, 20);
		}
	}
}



void ConditionManager::conditionValidationChanged(Condition*, int multiplexIndex)
{
	checkAllConditions(multiplexIndex);
}

void ConditionManager::onContainerParameterChanged(Parameter * p)
{
	if (p == conditionOperator)
	{
		for (int i = 0; i < getMultiplexCount(); i++) checkAllConditions(i);
	}
	else if (p == validationTime)
	{
		validationProgressFeedback->setEnabled(validationTime->floatValue() > 0);
	}
}

void ConditionManager::timerCallback(int id)
{
	if (validationTime->floatValue() == 0)
	{
		setValid(id, true);
		stopTimer(id);
		return;
	}

	double curTime = Time::getMillisecondCounterHiRes() / 1000.0;
	float diffProgress = (curTime - prevTimerTimes[id]) / validationTime->floatValue();
	float progress = validationProgresses[id] + diffProgress;
	setValidationProgress(id, progress);
	prevTimerTimes.set(id, curTime);

	if (validationProgresses[id] >= 1)
	{
		setValid(id, true);
		stopTimer(id);
	}
}

void ConditionManager::afterLoadJSONDataInternal()
{
	for (int i = 0; i < getMultiplexCount(); i++) checkAllConditions(i);
}

bool ConditionManager::areAllConditionsValid(int multiplexIndex, bool emptyIsValid)
{
	if (items.size() == 0) return emptyIsValid;

	int conditionsChecked = 0;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (!c->getIsValid(multiplexIndex)) return false;
		conditionsChecked++;
	}

	if (conditionsChecked == 0) return emptyIsValid;
	return true;
}

bool ConditionManager::isAtLeastOneConditionValid(int multiplexIndex, bool emptyIsValid)
{
	if (items.size() == 0) return emptyIsValid;

	int conditionsChecked = 0; 
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->getIsValid(multiplexIndex)) return true;
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

int ConditionManager::getNumValidConditions(int multiplexIndex)
{
	int result = 0;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->getIsValid(multiplexIndex)) result++;
	}
	return result;
}

bool ConditionManager::getIsValid(int multiplexIndex, bool emptyIsValid)
{
	return isValids[multiplexIndex] || (emptyIsValid && items.size() == 0);
}

void ConditionManager::dispatchConditionValidationChanged(int multiplexIndex)
{
	conditionManagerListeners.call(&ConditionManagerListener::conditionManagerValidationChanged, this, multiplexIndex);
	conditionManagerAsyncNotifier.addMessage(new ConditionManagerEvent(ConditionManagerEvent::VALIDATION_CHANGED, this, multiplexIndex));
}


InspectableEditor* ConditionManager::getEditor(bool isRoot)
{
	return new ConditionManagerEditor(this, isRoot);
}