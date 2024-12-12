/*
  ==============================================================================

	BaseComparator.cpp
	Created: 2 Nov 2016 8:54:51pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/ParameterLink/ParameterLink.h"
#include "BaseComparator.h"

BaseComparator::BaseComparator(Multiplex * multiplex) :
	ControllableContainer("Comparator"),
	MultiplexTarget(multiplex),
	reference(nullptr),
	comparatorNotifier(5)
{
	compareFunction = addEnumParameter("Comparison Function", "Decides what function checks the activeness of the condition");
	compareFunction->hideInEditor = true;
}


BaseComparator::~BaseComparator()
{
}

void BaseComparator::setReferenceParam(Parameter* p)
{
	if (p == reference) return;

	GenericScopedLock lock(compareLock);
	if (reference != nullptr)
	{
		removeControllable(reference);
		reference = nullptr;
	}

	reference = p;

	if (reference != nullptr)
	{
		addParameter(reference);
		if (isMultiplexed()) refLink.reset(new ParameterLink(reference, multiplex));
	}

	comparatorNotifier.addMessage(new ComparatorEvent(ComparatorEvent::REFERENCE_CHANGED, this));
}

void BaseComparator::addCompareOption(const String & name, const Identifier & func)
{
	compareFunction->addOption(name, var(func.toString()));
	if (compareFunction->enumValues.size() == 1) currentFunctionId = func.toString();
}


void BaseComparator::updateReferenceRange(Parameter* sourceParam)
{
	if (sourceParam->hasRange()) reference->setRange(sourceParam->minimumValue, sourceParam->maximumValue);
	else reference->clearRange();
}

var BaseComparator::getJSONData(bool includeNonOverriden)
{
	var data = ControllableContainer::getJSONData(includeNonOverriden);
	if(refLink != nullptr) data.getDynamicObject()->setProperty("refLink", refLink->getJSONData());
	return data;
}

void BaseComparator::loadJSONDataInternal(var data)
{
	if (refLink != nullptr) refLink->loadJSONData(data.getProperty("refLink", var()));
}

bool BaseComparator::compare(Parameter* sourceParam, int multiplexIndex)
{
	GenericScopedLock lock(compareLock);
	if (reference == nullptr || currentFunctionId.isNull()) return false;
	if (currentFunctionId == changeId) return true;

	return compareInternal(sourceParam, multiplexIndex);
}


void BaseComparator::onContainerParameterChanged(Parameter * p)
{
	if (p == compareFunction)
	{
		GenericScopedLock lock(compareLock);
		currentFunctionId = compareFunction->getValueData().toString();
		compareFunctionChanged();

	}
	ControllableContainer::onContainerParameterChanged(p);
}

BaseComparatorUI * BaseComparator::createUI()
{
	return new BaseComparatorUI(this);
}