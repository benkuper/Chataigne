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

ConditionManager::ConditionManager(IteratorProcessor * iterator) :
	IterativeTarget(iterator),
	BaseManager<Condition>("Conditions"),
	activateDef(nullptr),
	deactivateDef(nullptr),
    forceDisabled(false),
	conditionManagerAsyncNotifier(10)
{
	canBeCopiedAndPasted = true;
	selectItemWhenCreated = false;

	managerFactory = &factory;
	factory.defs.add(IterativeTargetDefinition<Condition>::createDef<StandardCondition>("", StandardCondition::getTypeStringStatic(false), iterator));
	factory.defs.add(IterativeTargetDefinition<Condition>::createDef<ConditionGroup>("", ConditionGroup::getTypeStringStatic(), iterator));
	factory.defs.add(IterativeTargetDefinition<Condition>::createDef<ScriptCondition>("", ScriptCondition::getTypeStringStatic(), iterator));
	if (isIterative()) factory.defs.add(IterativeTargetDefinition<Condition>::createDef<StandardCondition>("", StandardCondition::getTypeStringStatic(true), iterator)->addParam("listMode",true));
	
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

void ConditionManager::iteratorCountChanged()
{
	isValids.resize(getIterationCount());
	validationProgresses.resize(getIterationCount());

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
		for (int i = 0; i < getIterationCount(); i++) checkAllConditions(i);
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

	for (int i = 0; i < getIterationCount(); i++) checkAllConditions(i);
}

void ConditionManager::setValid(int iterationIndex, bool value, bool dispatchOnlyOnValidationChange)
{
	if (isValids[iterationIndex] == value && dispatchOnlyOnValidationChange) return;
	isValids.set(iterationIndex, value);
	dispatchConditionValidationChanged(iterationIndex);
}

void ConditionManager::setValidationProgress(int iterationIndex, float value)
{
	validationProgresses.set(iterationIndex, value);
	if (!isIterative()) validationProgressFeedback->setValue(value);
}

void ConditionManager::forceCheck()
{
	for (auto& i : items) i->forceCheck();
}

void ConditionManager::checkAllConditions(int iterationIndex, bool emptyIsValid, bool dispatchOnlyOnValidationChange)
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
		setValid(iterationIndex, valid, dispatchOnlyOnValidationChange);
	}else if (valid != validationWaitings[iterationIndex])
	{
		validationWaitings.set(iterationIndex, valid);
		setValidationProgress(iterationIndex, 0);
		setValid(iterationIndex, false);
		if(!valid)
		{
			stopTimer(iterationIndex);
		} else
		{
			prevTimerTimes.set(iterationIndex, Time::getMillisecondCounterHiRes() / 1000.0);
			startTimer(iterationIndex, 20);
		}
	}
}



void ConditionManager::conditionValidationChanged(Condition*, int iterationIndex)
{
	checkAllConditions(iterationIndex);
}

void ConditionManager::onContainerParameterChanged(Parameter * p)
{
	if (p == conditionOperator)
	{
		for (int i = 0; i < getIterationCount(); i++) checkAllConditions(i);
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
	for (int i = 0; i < getIterationCount(); i++) checkAllConditions(i);
}

bool ConditionManager::areAllConditionsValid(int iterationIndex, bool emptyIsValid)
{
	if (items.size() == 0) return emptyIsValid;

	int conditionsChecked = 0;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (!c->getIsValid(iterationIndex)) return false;
		conditionsChecked++;
	}

	if (conditionsChecked == 0) return emptyIsValid;
	return true;
}

bool ConditionManager::isAtLeastOneConditionValid(int iterationIndex, bool emptyIsValid)
{
	if (items.size() == 0) return emptyIsValid;

	int conditionsChecked = 0; 
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->getIsValid(iterationIndex)) return true;
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

int ConditionManager::getNumValidConditions(int iterationIndex)
{
	int result = 0;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->getIsValid(iterationIndex)) result++;
	}
	return result;
}

bool ConditionManager::getIsValid(int iterationIndex, bool emptyIsValid)
{
	return isValids[iterationIndex] || (emptyIsValid && items.size() == 0);
}

void ConditionManager::dispatchConditionValidationChanged(int iterationIndex)
{
	conditionManagerListeners.call(&ConditionManagerListener::conditionManagerValidationChanged, this, iterationIndex);
	conditionManagerAsyncNotifier.addMessage(new ConditionManagerEvent(ConditionManagerEvent::VALIDATION_CHANGED, this, iterationIndex));
}


InspectableEditor* ConditionManager::getEditor(bool isRoot)
{
	return new ConditionManagerEditor(this, isRoot);
}