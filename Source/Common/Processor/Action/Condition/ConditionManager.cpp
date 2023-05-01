/*
  ==============================================================================

	ConditionManager.cpp
	Created: 28 Oct 2016 8:07:18pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

ConditionManager::ConditionManager(Multiplex* multiplex) :
	MultiplexTarget(multiplex),
	BaseManager<Condition>("Conditions"),
	activateDef(nullptr),
	deactivateDef(nullptr),
	forceDisabled(false),
	useValidationProgress(false),
	isCheckingOtherConditionsWithSameSource(false),
	conditionManagerAsyncNotifier(10)
{
	canBeCopiedAndPasted = true;
	selectItemWhenCreated = false;

	isValids.resize(getMultiplexCount());
	validationProgresses.resize(getMultiplexCount());
	validationTargets.resize(getMultiplexCount());

	isValids.fill(false);
	validationProgresses.fill(0);
	validationTargets.fill(false);

	sequentialConditionIndices.resize(getMultiplexCount());

	factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<StandardCondition>("", StandardCondition::getTypeStringStatic(false), multiplex));
	if (isMultiplexed())
	{
		factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<StandardCondition>("", StandardCondition::getTypeStringStatic(true), multiplex)->addParam("listMode", true));
		factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<MultiplexIndexCondition>("", MultiplexIndexCondition::getTypeStringStatic(), multiplex));
	}

	factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<ConditionGroup>("", ConditionGroup::getTypeStringStatic(), multiplex));
	factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<ScriptCondition>("", ScriptCondition::getTypeStringStatic(), multiplex));
	factory.defs.add(MultiplexTargetDefinition<Condition>::createDef<ManualCondition>("", ManualCondition::getTypeStringStatic(), multiplex));

	managerFactory = &factory;

	validationTime = addFloatParameter("Validation Time", "If greater than 0, the conditions will be validated only if they remain valid for this amount of time, when coming from an invalid state", 0, 0, (float)INT32_MAX);
	validationTime->defaultUI = FloatParameter::TIME;

	invalidationTime = addFloatParameter("Invalidation Time", "If greater than 0, the conditions will be invalidated only if they remain invalid for this amount of time, when coming from a valid state", 0, 0, (float)INT32_MAX);
	invalidationTime->defaultUI = FloatParameter::TIME;

	validationProgressFeedback = addFloatParameter("Validation Progress", "The feedback of the progress if validation time is more than 0", 0, 0, 1, false);
	validationProgressFeedback->setControllableFeedbackOnly(true);

	conditionOperator = addEnumParameter("Operator", "Operator for this manager, will decides how the conditions are validated.\nAND will need all conditions to be true. \
OR will need at least one condition to be true. \
SEQUENTIAL will check the first, and when it gets validated, will check the next one,  etc. and then loop back to the first \
");
	conditionOperator->addOption("AND", ConditionOperator::AND);
	conditionOperator->addOption("OR", ConditionOperator::OR);
	conditionOperator->addOption("SEQUENTIAL", ConditionOperator::SEQUENTIAL);
	conditionOperator->hideInEditor = true;
}

ConditionManager::~ConditionManager()
{
}

void ConditionManager::multiplexCountChanged()
{
	isValids.resize(getMultiplexCount());
	validationProgresses.resize(getMultiplexCount());
	validationTargets.resize(getMultiplexCount());
	sequentialConditionIndices.resize(getMultiplexCount());

	isValids.fill(false);
	validationProgresses.fill(0);
	validationTargets.fill(false);
	sequentialConditionIndices.fill(0);
}

void ConditionManager::multiplexPreviewIndexChanged()
{
	conditionManagerAsyncNotifier.addMessage(new ConditionManagerEvent(ConditionManagerEvent::MULTIPLEX_PREVIEW_CHANGED, this));
}

bool ConditionManager::hasActivationDefinitions()
{
	return activateDef != nullptr || deactivateDef != nullptr;
}

void ConditionManager::setHasActivationDefinitions(bool hasActivation, bool hasDeactivation)
{
	if (hasActivation)
	{
		if (activateDef == nullptr)
		{
			activateDef = MultiplexTargetDefinition<Condition>::createDef<ActivationCondition>("", ActivationCondition::getTypeStringStatic(ActivationCondition::ON_ACTIVATE), multiplex)->addParam("type", ActivationCondition::ON_ACTIVATE);
			factory.defs.add(activateDef);
		}
	}
	else
	{
		factory.defs.removeObject(activateDef);
		activateDef = nullptr;
	}

	if (hasDeactivation)
	{
		if (deactivateDef == nullptr)
		{
			deactivateDef = MultiplexTargetDefinition<Condition>::createDef<ActivationCondition>("", ActivationCondition::getTypeStringStatic(ActivationCondition::ON_DEACTIVATE), multiplex)->addParam("type", ActivationCondition::ON_DEACTIVATE);
			factory.defs.add(deactivateDef);
		}
	}
	else
	{
		factory.defs.removeObject(deactivateDef);
		deactivateDef = nullptr;
	}

	factory.buildPopupMenu();
}

void ConditionManager::addItemInternal(Condition* c, var data)
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

void ConditionManager::removeItemInternal(Condition* c)
{
	c->removeConditionListener(this);
	conditionOperator->hideInEditor = items.size() <= 1;

	sequentialConditionIndices.fill(0);
	conditionManagerAsyncNotifier.addMessage(new ConditionManagerEvent(ConditionManagerEvent::SEQUENTIAL_CONDITION_INDEX_CHANGED, this));


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

	for (auto& i : items) i->setForceDisabled(value);

	for (int i = 0; i < getMultiplexCount(); i++) checkAllConditions(i);
}

void ConditionManager::setValid(int multiplexIndex, bool value, bool dispatchOnlyOnValidationChange)
{
	if (isValids[multiplexIndex] == value && dispatchOnlyOnValidationChange) return;
	isValids.set(multiplexIndex, value);

	dispatchConditionValidationChanged(multiplexIndex, dispatchOnlyOnValidationChange);

	if (isValids[multiplexIndex] && conditionOperator->getValueDataAsEnum<ConditionOperator>() == SEQUENTIAL)
	{
		int nextIndex = sequentialConditionIndices[multiplexIndex] + 1;
		while (nextIndex < items.size())
		{
			if (items[nextIndex]->enabled->boolValue()) break;
			nextIndex++;
		}

		if (nextIndex == items.size()) nextIndex = 0;

		setSequentialConditionIndices(nextIndex, multiplexIndex);
		setValid(multiplexIndex, false);
	}


}

void ConditionManager::setValidationProgress(int multiplexIndex, float value)
{
	validationProgresses.set(multiplexIndex, value);
	if (!isMultiplexed()) validationProgressFeedback->setValue(value);
}

void ConditionManager::setSequentialConditionIndices(int index, int multiplexIndex)
{
	if (multiplexIndex != -1) sequentialConditionIndices.set(multiplexIndex, index);
	else sequentialConditionIndices.fill(index);

	conditionManagerAsyncNotifier.addMessage(new ConditionManagerEvent(ConditionManagerEvent::SEQUENTIAL_CONDITION_INDEX_CHANGED, this));
}

void ConditionManager::forceCheck()
{
	for (auto& i : items) i->forceCheck();
}

void ConditionManager::checkAllConditions(int multiplexIndex, bool emptyIsValid, bool dispatchOnlyOnValidationChange, int sourceConditionIndex)
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

	case ConditionOperator::SEQUENTIAL:
		if (items.size() == 0) valid = emptyIsValid;
		else if (sourceConditionIndex == sequentialConditionIndices[multiplexIndex])
		{
			jassert(sequentialConditionIndices[multiplexIndex] < items.size() && items[sequentialConditionIndices[multiplexIndex]]->enabled->boolValue()); //just loop, shoult not happen
			valid = items[sequentialConditionIndices[multiplexIndex]]->getIsValid(multiplexIndex);
		}

	}

	if ((valid && validationTime->floatValue() == 0) || (!valid && invalidationTime->floatValue() == 0))
	{
		setValid(multiplexIndex, valid, dispatchOnlyOnValidationChange);
		validationTargets.set(multiplexIndex, valid);

		if (valid && invalidationTime->floatValue() > 0) setValidationProgress(multiplexIndex, 1);
		else if (!valid && validationTime->floatValue() > 0) setValidationProgress(multiplexIndex, 0);

	}
	else if (valid != validationTargets[multiplexIndex])
	{
		
		if (valid != isValids[multiplexIndex])
		{
			setValidationProgress(multiplexIndex, valid ? 0 : 1);
			validationTargets.set(multiplexIndex, valid);
			
			prevTimerTimes.set(multiplexIndex, Time::getMillisecondCounterHiRes() / 1000.0);
			startTimer(multiplexIndex, 20);
		}
		else
		{
			stopTimer(multiplexIndex);
			setValidationProgress(multiplexIndex, valid);
			validationTargets.set(multiplexIndex, valid);
		}

		//setValid(multiplexIndex, !validationTargets[multiplexIndex]);
		//if (!valid)
		//{
		//	stopTimer(multiplexIndex);
		//}
		//else
		//{
		//	prevTimerTimes.set(multiplexIndex, Time::getMillisecondCounterHiRes() / 1000.0);
		//	startTimer(multiplexIndex, 20);
		//}
	}
}



void ConditionManager::conditionValidationChanged(Condition* c, int multiplexIndex, bool dispatchOnChangeOnly)
{
	if (isCheckingOtherConditionsWithSameSource) return;

	if (StandardCondition* sc = dynamic_cast<StandardCondition*>(c))
	{
		isCheckingOtherConditionsWithSameSource = true;
		for (auto& i : items)
		{
			if (i == c) continue;
			if (StandardCondition* otherSC = dynamic_cast<StandardCondition*>(i))
			{
				if (sc->sourceControllable == otherSC->sourceControllable) otherSC->checkComparator(multiplexIndex);
			}
		}
		isCheckingOtherConditionsWithSameSource = false;
	}

	checkAllConditions(multiplexIndex, false, dispatchOnChangeOnly, items.indexOf(c));
}

void ConditionManager::onContainerParameterChanged(Parameter* p)
{
	if (p == conditionOperator)
	{
		setSequentialConditionIndices(0);

		for (int i = 0; i < getMultiplexCount(); i++)
		{
			checkAllConditions(i);
		}
	}
	else if (p == validationTime || p == invalidationTime)
	{
		useValidationProgress = validationTime->floatValue() > 0 || invalidationTime->floatValue() > 0;
		validationProgressFeedback->setEnabled(useValidationProgress);
	}
}

void ConditionManager::timerCallback(int id)
{
	if (!useValidationProgress)
	{
		setValid(id, validationTargets[id]);
		stopTimer(id);
		return;
	}

	bool targetIsValid = validationTargets[id];


	double curTime = Time::getMillisecondCounterHiRes() / 1000.0;
	float diffProgress = (curTime - prevTimerTimes[id]) / (targetIsValid ? validationTime->floatValue() : invalidationTime->floatValue());

	if (!targetIsValid) diffProgress = -diffProgress;
	float progress = jlimit<float>(0, 1, validationProgresses[id] + diffProgress);

	setValidationProgress(id, progress);
	prevTimerTimes.set(id, curTime);

	if (validationProgresses[id] == (int)targetIsValid)
	{
		setValid(id, validationTargets[id]);
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
	for (auto& c : items)
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
	for (auto& c : items)
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
	for (auto& c : items)
	{
		if (c->enabled->boolValue()) result++;
	}
	return result;
}

int ConditionManager::getNumValidConditions(int multiplexIndex)
{
	int result = 0;
	for (auto& c : items)
	{
		if (!c->enabled->boolValue()) continue;
		if (c->getIsValid(multiplexIndex)) result++;
	}
	return result;
}

bool ConditionManager::getIsValid(int multiplexIndex, bool emptyIsValid)
{
	return (multiplexIndex >= 0 && isValids[multiplexIndex]) || (emptyIsValid && items.size() == 0);
}

void ConditionManager::dispatchConditionValidationChanged(int multiplexIndex, bool dispatchOnlyOnValidationChange)
{
	conditionManagerListeners.call(&ConditionManagerListener::conditionManagerValidationChanged, this, multiplexIndex, dispatchOnlyOnValidationChange);
	conditionManagerAsyncNotifier.addMessage(new ConditionManagerEvent(ConditionManagerEvent::VALIDATION_CHANGED, this, multiplexIndex));
}


InspectableEditor* ConditionManager::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new ConditionManagerEditor(this, isRoot);
}
