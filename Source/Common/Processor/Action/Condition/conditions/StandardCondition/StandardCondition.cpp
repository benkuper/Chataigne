/*
  ==============================================================================

	StandardCondition.cpp
	Created: 21 Feb 2017 11:37:26am
	Author:  Ben

  ==============================================================================
*/

#include "StandardCondition.h"
#include "Comparator/ComparatorFactory.h"
#include "ui/StandardConditionEditor.h"
#include "Module/ModuleManager.h"

StandardCondition::StandardCondition(var params, IteratorProcessor* processor) :
	Condition(getTypeStringStatic(params.getProperty("listMode", false)), params, processor),
	iteratorListMode(params.getProperty("listMode", false))
{
	sourceTarget = addTargetParameter("Input Value", "Element that will be the source to check if condition is active or not"); 

	if (iteratorListMode)
	{
		sourceTarget->targetType = TargetParameter::CONTAINER;
		sourceTarget->setRootContainer(&iterator->listManager);

		std::function<ControllableContainer* ()> getListFunc = std::bind(&IteratorProcessor::showAndGetList, iterator);
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
	updateSourceControllablesFromTarget();
}

void StandardCondition::iteratorCountChanged()
{
	Condition::iteratorCountChanged();
	rawIsValids.fill(false);
	forceToggleState(false);
}

void StandardCondition::setValid(int iterationIndex, bool value, bool dispatchOnChangeOnly)
{
	if (toggleMode->boolValue())
	{
		if (rawIsValids[iterationIndex] == value) return;
		rawIsValids.set(iterationIndex, value);

		if (rawIsValids[iterationIndex]) Condition::setValid(iterationIndex, !getIsValid(iterationIndex));
	}
	else
	{
		Condition::setValid(iterationIndex, value, dispatchOnChangeOnly && !alwaysTrigger->boolValue());
	}
}


void StandardCondition::checkComparator(int iterationIndex)
{
	Controllable* c = sourceControllables[iterationIndex].get();

	if (c->type == Controllable::TRIGGER) {
		setValid(iterationIndex, true);
		setValid(iterationIndex, false);
	}
	else
	{
		setValid(iterationIndex, comparator->compare((Parameter*)c));
	}
}

void StandardCondition::forceToggleState(bool value)
{
	rawIsValids.fill(value);
	for (int i = 0; i < getIterationCount(); i++)
	{
		Condition::setValid(i, value); //bypass standard condition to avoid toggle, and force dispatch
	}
}

void StandardCondition::updateSourceControllablesFromTarget()
{
	/*
	for (auto& sc : sourceControllables)
	{
		if (!sc.wasObjectDeleted() && sc != nullptr)
		{
			Module* m = ControllableUtil::findParentAs<Module>(sc);
			if (m != nullptr) unregisterLinkedInspectable(m);
		}
	}
	*/

	for (auto& sc : sourceControllables)
	{
		if (Parameter* sp = dynamic_cast<Parameter*>(sc.get())) sp->removeParameterListener(this);
		else if (Trigger* st = dynamic_cast<Trigger*>(sc.get())) st->removeTriggerListener(this);
	}
	
	sourceIndexMap.clear();


	if (isClearing) return;

	Array<WeakReference<Controllable>> newSources;
	if (!iteratorListMode)
	{
		if (sourceTarget->target != nullptr && !sourceTarget->target.wasObjectDeleted()) newSources.add(sourceTarget->target);
	}
	else
	{
		if (BaseIteratorList* list = dynamic_cast<BaseIteratorList*>(sourceTarget->targetContainer.get())) newSources.addArray(list->list);
	}

	bool rebuildComparator = true;
	if ((sourceControllables.size() > 0 && newSources.size() > 0) && sourceControllables[0] == newSources[0]) rebuildComparator = false;
	
	sourceControllables = newSources;

	int index = 0;
	for (auto& sc : sourceControllables)
	{
		if (Parameter* sp = dynamic_cast<Parameter*>(sc.get())) sp->addParameterListener(this);
		else if (Trigger* st = dynamic_cast<Trigger*>(sc.get())) st->addTriggerListener(this);
		sourceIndexMap.set(sc.get(), index++);
	}

	if (sourceControllables.size()  > 0)
	{
		if (rebuildComparator)
		{
			var oldData = var();
			if (comparator != nullptr) oldData = comparator->getJSONData();
			if (comparator != nullptr)
			{
				removeChildControllableContainer(comparator.get());
				comparator.reset();
			}

			if (sourceControllables[0]->type != Controllable::TRIGGER) comparator.reset(ComparatorFactory::createComparatorForControllable((Parameter*)sourceControllables[0].get()));

			/*
			for (auto& sc : sourceControllables)
			{
				Module* m = ControllableUtil::findParentAs<Module>(sc);
				if (m != nullptr) registerLinkedInspectable(m);
			}
			*/

			if (comparator != nullptr)
			{
				addChildControllableContainer(comparator.get());

				if (!isCurrentlyLoadingData)
				{
					if (!loadingComparatorData.isVoid())
					{
						comparator->loadJSONData(loadingComparatorData);
						//loadingComparatorData = var();
					}
					else if (!oldData.isVoid()) comparator->loadJSONData(oldData);
				}

				comparator->hideInEditor = true;
				for (int i = 0; i < getIterationCount(); i++) checkComparator(i);
			}
		}
	} else
	{
		if (comparator != nullptr)
		{
			loadingComparatorData = comparator->getJSONData();
			removeChildControllableContainer(comparator.get());
			comparator.reset();
		}
	}

	conditionListeners.call(&ConditionListener::conditionSourceChanged, this);
	conditionAsyncNotifier.addMessage(new ConditionEvent(ConditionEvent::SOURCE_CHANGED, this));
}

void StandardCondition::onContainerParameterChangedInternal(Parameter * p)
{
	Condition::onContainerParameterChangedInternal(p);

	if (p == sourceTarget && sourceTarget != nullptr)
	{
		updateSourceControllablesFromTarget();
	}
}

void StandardCondition::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Condition::onControllableFeedbackUpdateInternal(cc, c);
	if (comparator != nullptr && c == comparator->reference)
	{
		for (int i = 0; i < getIterationCount(); i++) checkComparator(i);
	}
}

void StandardCondition::onExternalParameterValueChanged(Parameter* p)
{
	if (sourceIndexMap.contains(p)) checkComparator(sourceIndexMap[p]);
	else Condition::onExternalParameterValueChanged(p);
}

void StandardCondition::onExternalParameterRangeChanged(Parameter* p)
{
	if (sourceIndexMap.contains(p) && sourceIndexMap[p] == 0)
	{
		if (comparator != nullptr) comparator->updateReferenceRange(p);
	}
	else Condition::onExternalParameterRangeChanged(p);
}

void StandardCondition::onExternalTriggerTriggered(Trigger* t)
{
	if (sourceIndexMap.contains(t)) checkComparator(sourceIndexMap[t]);
	else Condition::onExternalTriggerTriggered(t);
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



InspectableEditor * StandardCondition::getEditor(bool isRoot)
{
	return new StandardConditionEditor(this, isRoot);
}
