/*
  ==============================================================================

    Condition.cpp
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "Condition.h"
#include "Comparator/ComparatorFactory.h"
#include "ui/ConditionEditor.h"
#include "Module/ModuleManager.h"

Condition::Condition(const String &n, var /*params*/) :
	BaseItem(n),
    forceDisabled(false),
	conditionAsyncNotifier(10)
{
	isSelectable = false;
	isValid = addBoolParameter("Is Valid", "Where the condition passed the test or not.", false);
	isValid->isEditable = false;
	isValid->isSavable = false;
	isValid->hideInEditor = true;
}

Condition::~Condition()
{
}

void Condition::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);

	if (p == isValid)
	{
		if (!forceDisabled)
		{
			conditionListeners.call(&ConditionListener::conditionValidationChanged, this);
			conditionAsyncNotifier.addMessage(new ConditionEvent(ConditionEvent::VALIDATION_CHANGED,this));
		}
	}else if (p == enabled)
	{
		isValid->setValue(false);
	}
}

void Condition::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	isValid->setEnabled(!forceDisabled);
}

InspectableEditor * Condition::getEditor(bool isRoot)
{
	return new ConditionEditor(this, isRoot);
}
