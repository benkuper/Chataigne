/*
  ==============================================================================

	BaseComparator.cpp
	Created: 2 Nov 2016 8:54:51pm
	Author:  bkupe

  ==============================================================================
*/

#include "BaseComparator.h"
#include "ui/BaseComparatorUI.h"

BaseComparator::BaseComparator() :
	ControllableContainer("Comparator"),
	reference(nullptr)
{
	compareFunction = addEnumParameter("Comparison Function", "Decides what function checks the activeness of the condition");
	compareFunction->hideInEditor = true;
	compareFunction->hideInOutliner = true;
}


BaseComparator::~BaseComparator()
{
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


/*
TriggerComparator::TriggerComparator(Array<WeakReference<Controllable>> sources) :
	BaseComparator(sources)
{
	addCompareOption("Triggered", triggeredId);

	for (auto& s : sources)
	{
		((Trigger *)s.get())->addTriggerListener(this);
	}
}

TriggerComparator::~TriggerComparator()
{
	for (auto& s : sourceTriggers)
	{
		if(!s.wasObjectDeleted()) s->removeTriggerListener(this);
	}
}

void TriggerComparator::compare(int iterationIndex)
{
	setValid(iterationIndex, true);
	setValid(iterationIndex, false);
}

void TriggerComparator::triggerTriggered(Trigger * t) 
{ 
	if (sourceTriggers.contains(t)) compare(sourceTriggers.indexOf(t));
	else BaseComparator::triggerTriggered(t);
}

ParameterComparator::ParameterComparator(Array<WeakReference<Controllable>> sources) :
	BaseComparator(sources)
{
	for (auto& s : sources)
	{
		sourceParameters.add((Parameter*)s.get());
		((Parameter *)s.get())->addParameterListener(this);
	}
}

ParameterComparator::~ParameterComparator()
{
	for (auto& s : sourceParameters)
	{
		if (!s.wasObjectDeleted()) s->removeParameterListener(this);
	}
}

void ParameterComparator::onExternalParameterValueChanged(Parameter * p)
{
	if (sourceParameters.contains(p)) compare(sourceParameters.indexOf(p));
}
*/