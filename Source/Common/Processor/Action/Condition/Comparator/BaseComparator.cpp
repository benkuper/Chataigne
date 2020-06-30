/*
  ==============================================================================

	BaseComparator.cpp
	Created: 2 Nov 2016 8:54:51pm
	Author:  bkupe

  ==============================================================================
*/

#include "BaseComparator.h"
#include "ui/BaseComparatorUI.h"

BaseComparator::BaseComparator(Controllable* _source) :
	ControllableContainer("Comparator"),
	isValid(false),
	rawIsValid(false),
	source(_source),
	reference(nullptr)
{
	compareFunction = addEnumParameter("Comparison Function", "Decides what function checks the activeness of the condition");
	compareFunction->hideInEditor = true;
	compareFunction->hideInOutliner = true;

	if (Parameter* p = dynamic_cast<Parameter*>(source)) p->addParameterListener(this);

	toggleMode = addBoolParameter("Toggle Mode", "If checked, this will make a validation alternate between validated and invalidated, useful to transform straight values into toggles", false);
	alwaysTrigger = addBoolParameter("Always Trigger", "If NOT checked the comparator notifies only when VALIDITY changes. If checked, the comparator notifies everytime the comparator is checked, meaning everytime the value is changed.",false);
}

BaseComparator::~BaseComparator()
{
	masterReference.clear();
	if (Parameter* p = dynamic_cast<Parameter*>(source)) p->removeParameterListener(this);
}

void BaseComparator::setValid(bool value)
{
	if (toggleMode->boolValue())
	{
		if (rawIsValid == value) return;
		rawIsValid = value;
		
		if (!rawIsValid) return;
		isValid = !isValid;
	}
	else
	{
		if (isValid == value && !alwaysTrigger->boolValue()) return;
		isValid = value;
	}

	comparatorListeners.call(&ComparatorListener::comparatorValidationChanged, this);
}

void BaseComparator::addCompareOption(const String & name, const Identifier & func)
{
	compareFunction->addOption(name, var(func.toString()));
	if (compareFunction->enumValues.size() == 1) compareFunction->setValue(name);
}

void BaseComparator::forceLeastPriority()
{
	if (source == nullptr) return;

	if (source->type == Controllable::TRIGGER)
	{
		((Trigger*)source)->removeTriggerListener(this);
		((Trigger*)source)->addTriggerListener(this);
	}
	else
	{
		((Parameter*)source)->removeParameterListener(this);
		((Parameter*)source)->addParameterListener(this);
	}
}

void BaseComparator::onContainerParameterChanged(Parameter * p)
{
	if (p == compareFunction)
	{
		if (compareFunction->getValueData().toString().isNotEmpty())
		{
			currentFunctionId = compareFunction->getValueData().toString();
			if(!isCurrentlyLoadingData) compare();
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
