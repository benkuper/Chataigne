/*
  ==============================================================================

    Condition.cpp
    Created: 28 Oct 2016 8:06:58pm
    Author:  bkupe

  ==============================================================================
*/

#include "Condition.h"
#include "ui/ConditionEditor.h"
#include "Module/ModuleManager.h"

Condition::Condition(const String& n, var params, IteratorProcessor * iterator) :
	BaseItem(n),
	IterativeTarget(iterator),
	forceDisabled(false),
	conditionAsyncNotifier(30)
{
	isSelectable = false;
}

Condition::~Condition()
{
}

void Condition::iteratorCountChanged()
{
	isValids.resize(getIterationCount());
	isValids.fill(false);
}

bool Condition::getIsValid(int iterationIndex)
{
	return isValids[iterationIndex];
}

void Condition::setValid(int iterationIndex, bool value, bool dispatchOnChangeOnly)
{
	if (isValids[iterationIndex] == value && dispatchOnChangeOnly) return;
	isValids.set(iterationIndex, value);

	conditionListeners.call(&ConditionListener::conditionValidationChanged, this, iterationIndex);
	conditionAsyncNotifier.addMessage(new ConditionEvent(ConditionEvent::VALIDATION_CHANGED, this, iterationIndex));
}

void Condition::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		for (int i = 0; i < getIterationCount(); i++) setValid(i, false);
	}
}

void Condition::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
}

InspectableEditor * Condition::getEditor(bool isRoot)
{
	return new ConditionEditor(this, isRoot);
}
