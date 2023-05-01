/*
  ==============================================================================

	StandardCondition.cpp
	Created: 21 Feb 2017 11:37:26am
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"

StandardCondition::StandardCondition(var params, Multiplex* processor) :
	Condition(getTypeStringStatic(params.getProperty("listMode", false)), params, processor),
	multiplexListMode(params.getProperty("listMode", false)),
	sourceList(nullptr)
{
	sourceTarget = addTargetParameter("Input Value", "Element that will be the source to check if condition is active or not");

	if (multiplexListMode)
	{
		sourceTarget->targetType = TargetParameter::CONTAINER;
		sourceTarget->setRootContainer(&multiplex->listManager);

		std::function<void(ControllableContainer*, std::function<void(ControllableContainer*)>)> getListFunc = std::bind(&Multiplex::showAndGetList, multiplex, std::placeholders::_1, std::placeholders::_2);
		sourceTarget->customGetTargetContainerFunc = getListFunc;
		sourceTarget->showParentNameInEditor = false;
	}
	else
	{
		sourceTarget->customGetTargetFunc = &ModuleManager::showAllValuesAndGetControllable;
		sourceTarget->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable;
		sourceTarget->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;
	}

	sourceTarget->hideInEditor = true;

	toggleMode = addBoolParameter("Toggle Mode", "If checked, this will make a validation alternate between validated and invalidated, useful to transform straight values into toggles", false);
	toggleMode->hideInEditor = true;

	alwaysTrigger = addBoolParameter("Always Trigger", "If NOT checked the comparator notifies only when VALIDITY changes. If checked, the comparator notifies everytime the comparator is checked, meaning everytime the value is changed.", false);
	alwaysTrigger->hideInEditor = true;
}

StandardCondition::~StandardCondition()
{
}

void StandardCondition::clearItem()
{
	BaseItem::clearItem();
	if (sourceList != nullptr) sourceList->removeListListener(this);
	if (sourceControllable != nullptr)
	{
		if (sourceControllable->type == Controllable::TRIGGER) ((Trigger*)sourceControllable.get())->removeTriggerListener(this);
		else ((Parameter*)sourceControllable.get())->removeParameterListener(this);
	}
}

void StandardCondition::multiplexCountChanged()
{
	Condition::multiplexCountChanged();
	rawIsValids.fill(false);
	forceToggleState(false);
}

void StandardCondition::setValid(int multiplexIndex, bool value, bool dispatchOnChangeOnly)
{
	if (toggleMode->boolValue())
	{
		if (rawIsValids[multiplexIndex] == value) return;
		rawIsValids.set(multiplexIndex, value);

		if (rawIsValids[multiplexIndex]) Condition::setValid(multiplexIndex, !getIsValid(multiplexIndex));
	}
	else
	{
		Condition::setValid(multiplexIndex, value, dispatchOnChangeOnly && !alwaysTrigger->boolValue());
	}
}


void StandardCondition::updateSourceFromTarget()
{
	if (multiplexListMode)
	{
		if (sourceList != nullptr)
		{
			sourceList->removeListListener(this);
		}

		sourceList = dynamic_cast<BaseMultiplexList*>(sourceTarget->targetContainer.get());

		if (sourceList != nullptr)
		{
			sourceList->addListListener(this);
		}
	}
	else
	{
		if (sourceControllable != nullptr)
		{
			if (sourceControllable->type == Controllable::TRIGGER) ((Trigger*)sourceControllable.get())->removeTriggerListener(this);
			else ((Parameter*)sourceControllable.get())->removeParameterListener(this);
		}

		sourceControllable = sourceTarget->target;

		if (sourceControllable != nullptr)
		{
			if (sourceControllable->type == Controllable::TRIGGER) ((Trigger*)sourceControllable.get())->addTriggerListener(this);
			else ((Parameter*)sourceControllable.get())->addParameterListener(this);
		}
	}

	updateComparatorFromSource();

	conditionListeners.call(&ConditionListener::conditionSourceChanged, this);
	conditionAsyncNotifier.addMessage(new ConditionEvent(ConditionEvent::SOURCE_CHANGED, this));
}

void StandardCondition::updateComparatorFromSource()
{
	if (Controllable* c = getSourceControllableAt(0))
	{
		bool rebuildComparator = comparator == nullptr || c->type != comparator->reference->type;
		if (rebuildComparator)
		{
			//var oldData = var();
			//if (comparator != nullptr) oldData = comparator->getJSONData();
			if (comparator != nullptr)
			{
				removeChildControllableContainer(comparator.get());
				comparator.reset();
			}

			if (c->type != Controllable::TRIGGER) comparator.reset(ComparatorFactory::createComparatorForControllable((Parameter*)c, multiplex));

			if (comparator != nullptr)
			{
				addChildControllableContainer(comparator.get());

				if (!isCurrentlyLoadingData)
				{
					if (!loadingComparatorData.isVoid())
					{
						comparator->loadJSONData(loadingComparatorData);
					}
					//else if (!oldData.isVoid()) comparator->loadJSONData(oldData);
				}

				comparator->hideInEditor = true;
				for (int i = 0; i < getMultiplexCount(); i++) checkComparator(i);
			}
		}
		else if (comparator != nullptr)
		{
			comparator->updateReferenceRange((Parameter*)c);
		}
	}
	else
	{
		if (comparator != nullptr)
		{
			loadingComparatorData = comparator->getJSONData();
			removeChildControllableContainer(comparator.get());
			comparator.reset();
		}
	}

}

Controllable* StandardCondition::getSourceControllableAt(int multiplexIndex)
{
	if (multiplexListMode) return sourceList != nullptr ? sourceList->getTargetControllableAt(multiplexIndex) : nullptr;
	else return sourceControllable;
}

void StandardCondition::checkComparator(int multiplexIndex)
{
	if (isCurrentlyLoadingData) return;

	if (Controllable* c = getSourceControllableAt(multiplexIndex))
	{
		if (c->type == Controllable::TRIGGER) {
			setValid(multiplexIndex, true);
			setValid(multiplexIndex, false);
		}
		else if (comparator != nullptr)
		{
			setValid(multiplexIndex, comparator->compare((Parameter*)c, multiplexIndex));
		}
	}
}

void StandardCondition::forceCheck()
{
	for (int i = 0; i < getMultiplexCount(); i++) checkComparator(i);
}

void StandardCondition::forceToggleState(bool value)
{
	rawIsValids.fill(value);
	for (int i = 0; i < getMultiplexCount(); i++)
	{
		Condition::setValid(i, value); //bypass standard condition to avoid toggle, and force dispatch
	}
}

void StandardCondition::listReferenceUpdated()
{
	updateComparatorFromSource();
}

void StandardCondition::listItemUpdated(int multiplexIndex)
{
	checkComparator(multiplexIndex);
}


void StandardCondition::onContainerParameterChangedInternal(Parameter* p)
{
	Condition::onContainerParameterChangedInternal(p);

	if (p == sourceTarget && sourceTarget != nullptr)
	{
		updateSourceFromTarget();
	}
}

void StandardCondition::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Condition::onControllableFeedbackUpdateInternal(cc, c);

	if (comparator != nullptr && (c == comparator->reference || c == comparator->compareFunction))
	{
		if (!isCurrentlyLoadingData) for (int i = 0; i < getMultiplexCount(); i++) checkComparator(i);
	}
}

void StandardCondition::onExternalParameterValueChanged(Parameter* p)
{
	if (!multiplexListMode)
	{
		for (int i = 0; i < getMultiplexCount(); i++) checkComparator(i);
	}
}

void StandardCondition::onExternalParameterRangeChanged(Parameter* p)
{
	if (!multiplexListMode && p == sourceControllable && comparator != nullptr) comparator->updateReferenceRange(p);
}

void StandardCondition::onExternalTriggerTriggered(Trigger* t)
{
	if (!multiplexListMode)
	{
		for (int i = 0; i < getMultiplexCount(); i++) checkComparator(i);

	}
}

var StandardCondition::getJSONData()
{
	var data = Condition::getJSONData();
	if (comparator != nullptr) data.getDynamicObject()->setProperty("comparator", comparator->getJSONData());
	return data;
}

void StandardCondition::loadJSONDataInternal(var data)
{
	Condition::loadJSONDataInternal(data);
	if (comparator != nullptr)
	{
		comparator->loadJSONData(data.getProperty("comparator", var()));
	}
	else if (Engine::mainEngine->isLoadingFile)
	{
		loadingComparatorData = data.getProperty("comparator", var());
	}
}

void StandardCondition::afterLoadJSONDataInternal()
{
	Condition::afterLoadJSONDataInternal();
	for (int i = 0; i < getMultiplexCount(); i++) checkComparator(i);
}



InspectableEditor* StandardCondition::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new StandardConditionEditor(this, isRoot);
}
