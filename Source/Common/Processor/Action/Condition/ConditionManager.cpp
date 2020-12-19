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
#include "conditions/IterativeCondition/IterativeCondition.h"
#include "Common/Processor/Action/Action.h"

juce_ImplementSingleton(ConditionManager)

ConditionManager::ConditionManager() :
	BaseManager<Condition>("Conditions"),
	activateDef(nullptr),
	deactivateDef(nullptr),
	iterativeDef(nullptr),
    forceDisabled(false),
	iterationCount(0)
{
	canBeCopiedAndPasted = true;

	managerFactory = &factory;
	factory.defs.add(Factory<Condition>::Definition::createDef<StandardCondition>("", StandardCondition::getTypeStringStatic()));
	factory.defs.add(Factory<Condition>::Definition::createDef<ConditionGroup>("", ConditionGroup::getTypeStringStatic()));
	factory.defs.add(Factory<Condition>::Definition::createDef<ScriptCondition>("", ScriptCondition::getTypeStringStatic()));


	validationTime = addFloatParameter("Validation Time", "If greater than 0, the conditions will be validated only if they remain valid for this amount of time", 0, 0, (float)INT32_MAX);
	validationTime->hideInEditor = true;
	validationTime->defaultUI = FloatParameter::TIME;

	selectItemWhenCreated = false;
	
	conditionOperator = addEnumParameter("Operator", "Operator for this manager, will decides how the conditions are validated");
	conditionOperator->addOption("AND", ConditionOperator::AND);
	conditionOperator->addOption("OR", ConditionOperator::OR);
	conditionOperator->hideInEditor = true;
}

ConditionManager::~ConditionManager()
{
}

void ConditionManager::setIterationCount(int count)
{
	if (iterationCount == count) return;

	iterationCount = count;

	while (iterationCount < isValids.size())
	{
		removeControllable(isValids[isValids.size() - 1]);
		isValids.removeLast();
		removeControllable(validationProgresses[validationProgresses.size() - 1]);
		validationProgresses.removeLast();

		validationWaitings.removeLast();
		prevTimerTimes.removeLast();
	}

	while (iterationCount > isValids.size())
	{
		BoolParameter * isValid = addBoolParameter("Is Valid", "Indicates if all the conditions are valid. If so, the consequences are triggered one time, at the moment the action becomes valid.", false);
		isValid->isControllableFeedbackOnly = true;
		isValid->hideInEditor = true;
		isValid->isSavable = false;
		isValids.add(isValid);

		FloatParameter * validationProgress = addFloatParameter("Progress", "Validation time progress", 0, 0, validationTime->floatValue());
		validationProgress->setControllableFeedbackOnly(true);
		validationProgress->setEnabled(false);
		validationProgress->hideInEditor = true;
		validationProgress->isSavable = false;
		validationProgresses.add(validationProgress);

		validationWaitings.add(false);
		prevTimerTimes.add(0);
	}

	if (iterationCount  > 0) validationProgresses[0]->hideInEditor = iterationCount > 0;

	for (auto& i : items) i->setIterationCount(iterationCount);
	
	if (iterationCount > 1)
	{
		if (iterativeDef == nullptr)
		{
			iterativeDef = Factory<Condition>::Definition::createDef<IterativeCondition>("", IterativeCondition::getTypeStringStatic());
			factory.defs.add(iterativeDef);
			factory.buildPopupMenu();
		}
		
	}
	else if(iterativeDef != nullptr)
	{
		factory.defs.removeObject(iterativeDef);
		iterativeDef = nullptr;
		factory.buildPopupMenu();
	}

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
		for (int i = 0; i < iterationCount; i++) checkAllConditions(i);
	}
}

void ConditionManager::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return; 
	forceDisabled = value;
	if (forceDisabled)
	{
		for (auto& v : isValids) v->setValue(false);
	}

	for (auto &i : items) i->setForceDisabled(value);

	for (int i = 0; i < iterationCount; i++) checkAllConditions(i);
}

void ConditionManager::forceCheck()
{
	for (auto& i : items) i->forceCheck();
}

void ConditionManager::checkAllConditions(int iterationIndex, bool emptyIsValid, bool dispatchOnlyOnValidationChange)
{
	jassert(iterationIndex > iterationCount);

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
		if (valid != isValids[iterationIndex]->boolValue() || dispatchOnlyOnValidationChange)
		{
			isValids[iterationIndex]->setValue(valid);
			dispatchConditionValidationChanged();
		}
	}else if (valid != validationWaitings[iterationIndex])
	{
		validationWaitings.set(iterationIndex, valid);

		validationProgresses[iterationIndex]->setValue(0);
		isValids[iterationIndex]->setValue(false);

		if(!valid)
		{
			stopTimer(iterationIndex);
			dispatchConditionValidationChanged();
		} else
		{

			prevTimerTimes.set(iterationIndex, Time::getHighResolutionTicks());
			startTimer(iterationIndex, 20);
		}
	}
}



void ConditionManager::conditionValidationChanged(Condition*, const IterativeContext& context)
{
	checkAllConditions(context.indexInList);
}

void ConditionManager::onContainerParameterChanged(Parameter * p)
{
	if (p == validationTime)
	{
		for (int i = 0; i < iterationCount; i++)
		{
			validationProgresses[i]->setEnabled(validationTime->floatValue() > 0);
			validationProgresses[i]->setRange(0, validationTime->floatValue());
		}
	} else if (p == conditionOperator)
	{
		for (int i = 0; i < iterationCount; i++) checkAllConditions(i);
	}
}

void ConditionManager::timerCallback(int id)
{
	if (validationTime->floatValue() == 0)
	{
		isValids[id]->setValue(true);
		dispatchConditionValidationChanged();
		stopTimer(id);
		return;
	}

	double curTime = Time::getMillisecondCounterHiRes();
	validationProgresses[id]->setValue(validationProgresses[id]->floatValue() + (curTime - prevTimerTimes[id])/1000.f);
	prevTimerTimes.set(id, curTime);


	if (validationProgresses[id]->floatValue() >= validationTime->floatValue())
	{
		isValids[id]->setValue(true);
		dispatchConditionValidationChanged();
		stopTimer(id);
	}
}

void ConditionManager::afterLoadJSONDataInternal()
{
	for (int i = 0; i < iterationCount; i++) checkAllConditions(i);
}

bool ConditionManager::areAllConditionsValid(int iterationIndex, bool emptyIsValid)
{
	if (items.size() == 0) return emptyIsValid;

	int conditionsChecked = 0;
	for (auto &c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (!c->isValids[iterationIndex]->boolValue()) return false;
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
		if (c->isValids[iterationIndex]->boolValue()) return true;
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
		if (c->isValids[iterationIndex]->boolValue()) result++;
	}
	return result;
}

bool ConditionManager::getIsValid(int iterationIndex, bool emptyIsValid)
{
	return isValids[iterationIndex]->boolValue() || (emptyIsValid && items.size() == 0);
}

void ConditionManager::dispatchConditionValidationChanged()
{
	conditionManagerListeners.call(&ConditionManagerListener::conditionManagerValidationChanged, this);
}


InspectableEditor* ConditionManager::getEditor(bool isRoot)
{
	return new ConditionManagerEditor(this, isRoot);
}