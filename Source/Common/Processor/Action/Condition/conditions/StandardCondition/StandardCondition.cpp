/*
  ==============================================================================

	StandardCondition.cpp
	Created: 21 Feb 2017 11:37:26am
	Author:  Ben

  ==============================================================================
*/

#include "StandardCondition.h"
#include "../../Comparator/ComparatorFactory.h"
#include "ui/StandardConditionEditor.h"
#include "Module/ModuleManager.h"

StandardCondition::StandardCondition(var params) :
	Condition(getTypeString(),params)
{
	sourceTarget = addTargetParameter("Input Value", "Element that will be the source to check if condition is active or not"); 
	sourceTarget->customGetTargetFunc = &ModuleManager::showAllValuesAndGetControllable;
	sourceTarget->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable;
	sourceTarget->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;
	sourceTarget->hideInEditor = true;
}

StandardCondition::~StandardCondition()
{
	setSourceControllables(Array<WeakReference<Controllable>>());
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
		for (auto& v : isValids) v->alwaysNotify = comparator->alwaysTrigger->boolValue();
	}
	else if (Engine::mainEngine->isLoadingFile)
	{
		loadingComparatorData = data.getProperty("comparator", var());
	}
}


void StandardCondition::setSourceControllables(Array<WeakReference<Controllable>> newSources)
{
	for (auto& sc : sourceControllables)
	{
		if (!sc.wasObjectDeleted() && sc != nullptr)
		{
			Module* m = ControllableUtil::findParentAs<Module>(sc);
			if (m != nullptr) unregisterLinkedInspectable(m);
		}
	}

	sourceControllables = newSources;

	if (sourceControllables.size()  > 0)
	{
		var oldData = var();
		if (comparator != nullptr) oldData = comparator->getJSONData();
		
		if (comparator != nullptr)
		{
			removeChildControllableContainer(comparator.get());
		}

		comparator.reset(ComparatorFactory::createComparatorForControllable(sourceControllables));

		for (auto& sc : sourceControllables)
		{
			Module* m = ControllableUtil::findParentAs<Module>(sc);
			if (m != nullptr) registerLinkedInspectable(m);
		}

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
			comparator->addComparatorListener(this);
			
			for (int i = 0; i < iterationCount; i++)
			{
				comparator->compare(i);

				isValids[i]->alwaysNotify = comparator->alwaysTrigger->boolValue();
				isValids[i]->setValue(comparator->isValids[i]);
			}

		}
	} else
	{
		if (comparator != nullptr)
		{
			loadingComparatorData = comparator->getJSONData();
			comparator->removeComparatorListener(this);
			removeChildControllableContainer(comparator.get());
			comparator = nullptr;
			
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
		setSourceControllables(sourceTarget->target);
	}
}

void StandardCondition::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Condition::onControllableFeedbackUpdateInternal(cc, c);

	if (comparator != nullptr && c == comparator->alwaysTrigger)
	{
		for(auto & v : isValids) v->alwaysNotify = comparator->alwaysTrigger->boolValue();
	}
}

InspectableEditor * StandardCondition::getEditor(bool isRoot)
{
	return new StandardConditionEditor(this, isRoot);
}


void StandardCondition::setSourceControllables(Array<WeakReference<Controllable>> newSources)
{
}

void StandardCondition::comparatorValidationChanged(BaseComparator*, int iterationIndex)
{
	isValids[iterationIndex]->setValue(comparator->isValids[iterationIndex]);
}

void StandardCondition::forceCheck()
{
	if (comparator != nullptr)
	{
		comparator->forceCheck();
	}
}

