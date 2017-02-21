/*
  ==============================================================================

	StandardCondition.cpp
	Created: 21 Feb 2017 11:37:26am
	Author:  Ben

  ==============================================================================
*/

#include "StandardCondition.h"
#include "TargetParameter.h"
#include "StandardConditionEditor.h"
#include "ComparatorFactory.h"

StandardCondition::StandardCondition()
{
	sourceTarget = addTargetParameter("Source", "Element that will be the source to check if condition is active or not", ModuleManager::getInstance());
}

StandardCondition::~StandardCondition()
{
	setSourceControllable(nullptr);
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
	if (comparator != nullptr) comparator->loadJSONData(data.getProperty("comparator", var()));
}


void StandardCondition::setSourceControllable(WeakReference<Controllable> c)
{
	if (!sourceControllable.wasObjectDeleted() && sourceControllable != nullptr)
	{
		if (sourceControllable->type == Controllable::TRIGGER) ((Trigger *)sourceControllable.get())->removeTriggerListener(this);
		else ((Parameter *)sourceControllable.get())->removeParameterListener(this);
	}

	sourceControllable = c;


	if (sourceControllable != nullptr)
	{
		var oldData = var();
		if (comparator != nullptr) oldData = comparator->getJSONData();
		comparator = ComparatorFactory::createComparatorForControllable(sourceControllable);

		if (comparator != nullptr)
		{
			if (!oldData.isVoid()) comparator->loadJSONData(oldData);
			comparator->addComparatorListener(this);
			comparator->compare();
		}


	} else
	{
		if (comparator != nullptr)
		{
			comparator->removeComparatorListener(this);
			removeChildControllableContainer(comparator);
			comparator = nullptr;
		}
	}

	conditionListeners.call(&ConditionListener::conditionSourceChanged, this);
}

void StandardCondition::onContainerParameterChangedInternal(Parameter * p)
{
	Condition::onContainerParameterChangedInternal(p);
	if (p == sourceTarget)
	{
		setSourceControllable(sourceTarget->target);
	}
}


void StandardCondition::comparatorValidationChanged(BaseComparator *)
{
	isValid->setValue(comparator->isValid);

}


InspectableEditor * StandardCondition::getEditor(bool isRoot)
{
	return new StandardConditionEditor(this, isRoot);
}