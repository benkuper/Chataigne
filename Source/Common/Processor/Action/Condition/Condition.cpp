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

Condition::Condition(const String& n, var /*params*/) :
	BaseItem(n),
	forceDisabled(false),
	iterationCount(0),
	conditionAsyncNotifier(30)
{
	isSelectable = false;
}

Condition::~Condition()
{
}

void Condition::setIterationCount(int count)
{
	if (iterationCount == count) return;

	while (iterationCount < isValids.size())
	{
		removeControllable(isValids[isValids.size() - 1]);
		isValids.removeLast();
	}

	while (iterationCount > isValids.size())
	{
		BoolParameter * isValid = addBoolParameter("Is Valid", "Where the condition passed the test or not.", false);
		isValid->setControllableFeedbackOnly(true);
		isValid->hideInEditor = true;
		isValid->isSavable = false;
		isValids.add(isValid);
	}
}

void Condition::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);

	if (isValids.contains((BoolParameter *)p))
	{
		if (!forceDisabled)
		{
			IterativeContext context = { isValids.indexOf((BoolParameter*)p) };
			conditionListeners.call(&ConditionListener::conditionValidationChanged, this, context);
			conditionAsyncNotifier.addMessage(new ConditionEvent(ConditionEvent::VALIDATION_CHANGED, this, context));
		}
	}else if (p == enabled)
	{
		for (int i = 0; i < iterationCount; i++) isValids[i]->setValue(false);
	}
}

void Condition::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
	for (int i = 0; i < iterationCount; i++) isValids[i]->setEnabled(!forceDisabled);
}

InspectableEditor * Condition::getEditor(bool isRoot)
{
	return new ConditionEditor(this, isRoot);
}
