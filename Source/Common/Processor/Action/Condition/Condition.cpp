/*
  ==============================================================================

	Condition.cpp
	Created: 28 Oct 2016 8:06:58pm
	Author:  bkupe

  ==============================================================================
*/

//#include "Condition.h" //??

#include "Module/ModuleIncludes.h"

Condition::Condition(const String& n, var params, Multiplex* multiplex) :
	BaseItem(n),
	MultiplexTarget(multiplex),
	forceDisabled(false),
	conditionAsyncNotifier(30)
{
	isSelectable = false;

	isValids.resize(getMultiplexCount());
	isValids.fill(false);
}

Condition::~Condition()
{
}

void Condition::multiplexCountChanged()
{
	isValids.resize(getMultiplexCount());
	isValids.fill(false);
}

void Condition::multiplexPreviewIndexChanged()
{
	conditionAsyncNotifier.addMessage(new ConditionEvent(ConditionEvent::MULTIPLEX_PREVIEW_CHANGED, this));
}

bool Condition::getIsValid(int multiplexIndex)
{
	return isValids[multiplexIndex];
}

void Condition::setValid(int multiplexIndex, bool value, bool dispatchOnChangeOnly)
{
	if (isValids[multiplexIndex] == value && dispatchOnChangeOnly) return;
	isValids.set(multiplexIndex, value);

	conditionListeners.call(&ConditionListener::conditionValidationChanged, this, multiplexIndex, dispatchOnChangeOnly);
	conditionAsyncNotifier.addMessage(new ConditionEvent(ConditionEvent::VALIDATION_CHANGED, this, multiplexIndex));
}

void Condition::onContainerParameterChangedInternal(Parameter* p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		for (int i = 0; i < getMultiplexCount(); i++) setValid(i, false);
	}
}

void Condition::setForceDisabled(bool value, bool force)
{
	if (forceDisabled == value && !force) return;
	forceDisabled = value;
}

InspectableEditor* Condition::getEditorInternal(bool isRoot)
{
	return new ConditionEditor(this, isRoot);
}
