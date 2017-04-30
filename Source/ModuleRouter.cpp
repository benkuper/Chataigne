/*
  ==============================================================================

	ModuleRouter.cpp
	Created: 29 Apr 2017 10:35:18am
	Author:  Ben

  ==============================================================================
*/

#include "ModuleRouter.h"

ModuleRouter::ModuleRouter() :
	BaseItem("Router"),
	sourceModule(nullptr),
	destModule(nullptr)
{
    
}

ModuleRouter::~ModuleRouter()
{
	setSourceModule(nullptr);
	setDestModule(nullptr);
}

void ModuleRouter::setSourceModule(Module * m)
{
	if (m == sourceModule) return;
	if (sourceModule != nullptr)
	{
		sourceModule->removeInspectableListener(this);
		clearSourceValues();
	}
	
	sourceModule = m;

	if (sourceModule != nullptr)
	{
		sourceModule->addInspectableListener(this);
	}

	routerListeners.call(&RouterListener::sourceModuleChanged, this);
}

void ModuleRouter::setDestModule(Module * m)
{
	if (m == destModule) return;
	if (destModule != nullptr)
	{
		destModule->removeInspectableListener(this);
	}

	destModule = m;

	if (destModule != nullptr)
	{
		destModule->addInspectableListener(this);
	}
}

void ModuleRouter::setSourceValueSelected(Controllable * sourceValue, bool selected)
{
	int index = sourceFilteredValues.indexOf(sourceValue);
	bool found = index != -1;
	if (selected == found) return;

	if (selected)
	{
		sourceFilteredValues.add(sourceValue);
		sourceFilteredValueIndices.set(sourceValue,index);
		if (sourceValue->type == Controllable::TRIGGER) ((Trigger *)sourceValue)->addTriggerListener(this);
		else ((Parameter *)sourceValue)->addParameterListener(this);
	} else
	{
		sourceFilteredValues.removeAllInstancesOf(sourceValue);
		sourceFilteredValueIndices.remove(sourceValue);
		if (sourceValue->type == Controllable::TRIGGER) ((Trigger *)sourceValue)->removeTriggerListener(this);
		else ((Parameter *)sourceValue)->removeParameterListener(this);
	}
}

void ModuleRouter::clearSourceValues()
{
	while (sourceFilteredValues.size() > 0)
	{
		setSourceValueSelected(sourceFilteredValues[0], false);
	}
}

void ModuleRouter::inspectableDestroyed(Inspectable * i)
{
	if (i == sourceModule) setSourceModule(nullptr);
	else if (i == destModule) setDestModule(nullptr);
	else
	{
		Controllable * c = dynamic_cast<Controllable *>(i);
		if (c != nullptr)
		{
			if (sourceFilteredValues.indexOf(c) != -1)
			{
				setSourceValueSelected(c, false);
			}
		}
	}
}

void ModuleRouter::onExternalParameterChanged(Parameter * p)
{
	if (destModule == nullptr) return;
	destModule->handleRoutedModuleValue(p, sourceFilteredValueIndices[p]);
}

void ModuleRouter::onExternalTriggerTriggered(Trigger * t)
{
	if (destModule == nullptr) return;
	destModule->handleRoutedModuleValue(t, sourceFilteredValueIndices[t]);
}
