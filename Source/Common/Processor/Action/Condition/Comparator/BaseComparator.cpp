/*
  ==============================================================================

    BaseComparator.cpp
    Created: 2 Nov 2016 8:54:51pm
    Author:  bkupe

  ==============================================================================
*/

#include "BaseComparator.h"
#include "ui/BaseComparatorUI.h"

BaseComparator::BaseComparator(Controllable * _source) :
	ControllableContainer("Comparator"),
	isValid(false),
	source(_source),
	reference(nullptr)
{
	compareFunction = addEnumParameter("Comparison Function", "Decides what function checks the activeness of the condition");
	compareFunction->hideInEditor = true;
	compareFunction->hideInOutliner = true;

	Parameter * p = dynamic_cast<Parameter *>(source);
	if (p != nullptr)
	{
		p->addParameterListener(this);
	}

	//alwaysDispatch = addBoolParameter("Dispatch on change", "Whether the comparator notifies only when validity changes (OFF),  or each time the comparator is checked. (ON)",false);
}

BaseComparator::~BaseComparator()
{
	masterReference.clear();

	Parameter * p = dynamic_cast<Parameter *>(source);
	if (p != nullptr)
	{
		p->removeParameterListener(this);
	}
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
	if (compareFunction->enumValues.size() == 1) compareFunction->setValue(name,false,true);
}

void BaseComparator::onContainerParameterChanged(Parameter * p)
{
	if (p == compareFunction)
	{
		if (compareFunction->getValueData().toString().isNotEmpty())
		{
			currentFunctionId = compareFunction->getValueData().toString();
			compare();
		}
	} else if (p == reference)
	{
		compare();
	}
}

void BaseComparator::parameterRangeChanged(Parameter * p)
{
	if (p == source)
	{
		Parameter * sp = dynamic_cast<Parameter *>(source);
		Parameter * rp = dynamic_cast<Parameter *>(reference);
		if (rp != nullptr) rp->setRange(sp->minimumValue, sp->maximumValue);
	} else
	{
		ControllableContainer::parameterRangeChanged(p);
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
    if(!sourceParameter.wasObjectDeleted()) sourceParameter->removeParameterListener(this);
}

void ParameterComparator::onExternalParameterValueChanged(Parameter * p)
{
	if (p == sourceParameter) compare();
}
