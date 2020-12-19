/*
  ==============================================================================

	BaseComparator.cpp
	Created: 2 Nov 2016 8:54:51pm
	Author:  bkupe

  ==============================================================================
*/

#include "BaseComparator.h"
#include "ui/BaseComparatorUI.h"

BaseComparator::BaseComparator(Array<WeakReference<Controllable>> _sources) :
	ControllableContainer("Comparator"),
	reference(nullptr)
{
	compareFunction = addEnumParameter("Comparison Function", "Decides what function checks the activeness of the condition");
	compareFunction->hideInEditor = true;
	compareFunction->hideInOutliner = true;

	setSources(_sources);
	
	toggleMode = addBoolParameter("Toggle Mode", "If checked, this will make a validation alternate between validated and invalidated, useful to transform straight values into toggles", false);
	alwaysTrigger = addBoolParameter("Always Trigger", "If NOT checked the comparator notifies only when VALIDITY changes. If checked, the comparator notifies everytime the comparator is checked, meaning everytime the value is changed.",false);
}


BaseComparator::~BaseComparator()
{
	masterReference.clear();
	for (auto& s : sources)
	{
		if (s.wasObjectDeleted()) continue;
		if (Parameter* p = dynamic_cast<Parameter*>(s.get())) p->removeParameterListener(this);
	}
}

void BaseComparator::setValid(int iterationIndex, bool value)
{
	if (toggleMode->boolValue())
	{
		if (rawIsValids[iterationIndex] == value) return;
		rawIsValids.set(iterationIndex, value);
		
		if (!rawIsValids[iterationIndex]) return;
		isValids.set(iterationIndex, !isValids[iterationIndex]);
	}
	else
	{
		if (isValids[iterationIndex] == value && !alwaysTrigger->boolValue()) return;
		isValids.set(iterationIndex, value);
	}

	comparatorListeners.call(&ComparatorListener::comparatorValidationChanged, this, iterationIndex);
}

void BaseComparator::forceToggleState(bool value)
{
	for (int i = 0; i < sources.size(); i++)
	{
		isValids.set(i,value);
		rawIsValids.set(i,value);
		comparatorListeners.call(&ComparatorListener::comparatorValidationChanged, this, i);
	}
}

void BaseComparator::setSources(Array<WeakReference<Controllable>> newSources)
{
	int count = sources.size();

	if (newSources.size() == sources.size()) return;

	for (auto& s : sources)
	{
		if (s.wasObjectDeleted()) continue;
		if (Parameter* p = dynamic_cast<Parameter*>(sources[sources.size() - 1].get())) p->removeParameterListener(this);
	}

	sources.clear();
	isValids.clear();
	rawIsValids.clear();

	for (auto& s : newSources)
	{
		if (s.wasObjectDeleted()) continue;
		if (Parameter* p = dynamic_cast<Parameter*>(sources[sources.size() - 1].get())) p->addParameterListener(this);
		sources.add(s);

		isValids.add(false);
		rawIsValids.add(false);
	}
}

void BaseComparator::addCompareOption(const String & name, const Identifier & func)
{
	compareFunction->addOption(name, var(func.toString()));
	if (compareFunction->enumValues.size() == 1) currentFunctionId = func.toString();
}

void BaseComparator::forceCheck()
{
	for (int i = 0; i < sources.size();i++) compare(i);
}

void BaseComparator::onContainerParameterChanged(Parameter * p)
{
	if (p == compareFunction)
	{
		if (compareFunction->getValueData().toString().isNotEmpty())
		{
			currentFunctionId = compareFunction->getValueData().toString();
			if (!isCurrentlyLoadingData)
			{
				for (int i = 0; i < sources.size(); i++) compare(i);
			}
		}
	} else if (p == reference)
	{
		for (int i = 0; i < sources.size(); i++) compare(i);
	}
}

void BaseComparator::parameterRangeChanged(Parameter * p)
{
	if(sources.size() > 0 && p == sources[0])
	{
		Parameter * sp = dynamic_cast<Parameter *>(p);
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
