/*
  ==============================================================================

    Condition.cpp
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "Condition.h"
#include  "ComparatorFactory.h"
#include "ModuleManager.h"

Condition::Condition(const String &n, var /*params*/) :
	BaseItem(n),
    forceDisabled(false),
    validationAsyncNotifier(10)
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
			validationAsyncNotifier.addMessage(new ValidationAsyncEvent(isValid->boolValue()));
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
