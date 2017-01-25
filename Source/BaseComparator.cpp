/*
  ==============================================================================

    BaseComparator.cpp
    Created: 2 Nov 2016 8:54:51pm
    Author:  bkupe

  ==============================================================================
*/

#include "BaseComparator.h"
#include "BaseComparatorUI.h"

BaseComparator::BaseComparator(Controllable * _source) :
	ControllableContainer("Comparator"),
	source(_source),
	isValid(false)
{
	isTargettable = false;
	compareFunction = addEnumParameter("Comparison Function", "Decides what function checks the activeness of the condition");
	compareFunction->hideInEditor = true;
	compareFunction->hideInOutliner = true;
	compareFunction->isTargettable = false;

	//alwaysDispatch = addBoolParameter("Dispatch on change", "Whether the comparator notifies only when validity changes (OFF),  or each time the comparator is checked. (ON)",false);
}

BaseComparator::~BaseComparator()
{
}

void BaseComparator::setValid(bool value)
{
	if (isValid == value/* && alwaysDispatch->boolValue()*/) return;


	isValid = value;
	comparatorListeners.call(&ComparatorListener::comparatorValidationChanged, this);
}

void BaseComparator::addCompareOption(const String & name, const Identifier & func)
{
	compareFunction->addOption(name, var(func.toString()));
}

void BaseComparator::onContainerParameterChanged(Parameter * p)
{
	if (p == compareFunction)
	{
		currentFunctionId = compareFunction->getValueData().toString();
		compare();
	} else if (p == reference)
	{
		compare();
	}
}

BaseComparatorUI * BaseComparator::createUI()
{
	return new BaseComparatorUI(this);
}


TriggerComparator::TriggerComparator(Controllable * source) :
	BaseComparator(source),
	sourceTrigger((Trigger *)source)
{
	addCompareOption("Triggered", triggeredId);
	sourceTrigger->addTriggerListener(this);
}

TriggerComparator::~TriggerComparator()
{
	sourceTrigger->removeTriggerListener(this);
}

void TriggerComparator::compare()
{
	setValid(true);
	setValid(false);
}

void TriggerComparator::triggerTriggered(Trigger * t) { 
	if(t == sourceTrigger) compare(); 
	else BaseComparator::triggerTriggered(t);
}

ParameterComparator::ParameterComparator(Controllable * source) :
	BaseComparator(source),
	sourceParameter((Parameter *)source)
{
	sourceParameter->addParameterListener(this);
}

ParameterComparator::~ParameterComparator()
{
	sourceParameter->removeParameterListener(this);
}

void ParameterComparator::onExternalParameterChanged(Parameter * p)
{
	if (p == sourceParameter) compare();
}
