/*
  ==============================================================================

    Condition.cpp
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "Condition.h"
#include "TargetParameter.h"
#include  "ComparatorFactory.h"
#include "ModuleManager.h"
#include "ConditionEditor.h"

Condition::Condition() :
	BaseItem("Condition")
{
	isSelectable = false;

	isValid = addBoolParameter("Is Valid", "Where the condition passed the test or not.", false);
	isValid->isEditable = false;
	isValid->isSavable = false;

	sourceTarget = addTargetParameter("Source", "Element that will be the source to check if condition is active or not",ModuleManager::getInstance());
	

}

Condition::~Condition()
{
	setSourceControllable(nullptr);
}

var Condition::getJSONData()
{
	var data = BaseItem::getJSONData();
	if (comparator != nullptr) data.getDynamicObject()->setProperty("comparator", comparator->getJSONData());
	return data;
}

void Condition::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	if (comparator != nullptr) comparator->loadJSONData(data.getProperty("comparator", var()));
}

void Condition::setSourceControllable(WeakReference<Controllable> c)
{
	if (!sourceControllable.wasObjectDeleted() && sourceControllable != nullptr)
	{
		if(sourceControllable->type == Controllable::TRIGGER) ((Trigger *)sourceControllable.get())->removeTriggerListener(this);
		else ((Parameter *)sourceControllable.get())->removeParameterListener(this);
	}

	sourceControllable = c;
	

	if (sourceControllable != nullptr)
	{
		/*
		if (sourceControllable->type == Controllable::TRIGGER) ((Trigger *)c.get())->addTriggerListener(this);
		else ((Parameter *)sourceControllable.get())->addParameterListener(this);
		*/

		var oldData = var();
		if (comparator != nullptr) oldData = comparator->getJSONData();
		comparator = ComparatorFactory::createComparatorForControllable(sourceControllable);

		if (comparator != nullptr)
		{
			if(!oldData.isVoid()) comparator->loadJSONData(oldData);
			comparator->addComparatorListener(this);
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

void Condition::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == sourceTarget)
	{
		setSourceControllable(sourceTarget->target);
	} else if (p == isValid)
	{
		conditionListeners.call(&ConditionListener::conditionValidationChanged, this);
	}else if (p == enabled)
	{
		isValid->setValue(false);
	}
}

void Condition::comparatorValidationChanged(BaseComparator *)
{
	isValid->setValue(comparator->isValid);
	
}

InspectableEditor * Condition::getEditor(bool isRoot)
{
	return new ConditionEditor(this, isRoot);
}
