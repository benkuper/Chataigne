/*
  ==============================================================================

	BaseComparator.cpp
	Created: 2 Nov 2016 8:54:51pm
	Author:  bkupe

  ==============================================================================
*/

#include "Common/ParameterLink/ParameterLink.h"

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

var BaseComparator::getJSONData()
{
	var data = ControllableContainer::getJSONData();
	if(refLink != nullptr) data.getDynamicObject()->setProperty("refLink", refLink->getJSONData());
	return data;
}

void BaseComparator::loadJSONDataInternal(var data)
{
	if (refLink != nullptr) refLink->loadJSONData(data.getProperty("refLink", var()));
}


void BaseComparator::onContainerParameterChanged(Parameter * p)
{
	if (p == compareFunction)
	{
		currentFunctionId = compareFunction->getValueData().toString();
	}
	ControllableContainer::onContainerParameterChanged(p);
}

BaseComparatorUI * BaseComparator::createUI()
{
	return new BaseComparatorUI(this);
}