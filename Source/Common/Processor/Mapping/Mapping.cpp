/*
  ==============================================================================

	Mapping.cpp
	Created: 28 Oct 2016 8:06:13pm
	Author:  bkupe

  ==============================================================================
*/

#include "Mapping.h"
#include "ui/MappingUI.h"

Mapping::Mapping(bool canBeDisabled) :
	Processor("Mapping", canBeDisabled),
	inputIsLocked(false)
{
	itemDataType = "Mapping";
	type = MAPPING;

	continuousProcess = addBoolParameter("Continuous", "If enabled, the mapping will process continuously rather than only when parameter value has changed", false);
	continuousProcess->hideInEditor = true;

	addChildControllableContainer(&input);
	addChildControllableContainer(&cdm);
	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);
	
	fm.addBaseManagerListener(this);
	input.addMappingInputListener(this);

	helpID = "Mapping";
}

Mapping::~Mapping()
{
}

void Mapping::lockInputTo(Parameter * lockParam)
{
	inputIsLocked = lockParam != nullptr;
	input.lockInput(lockParam);
	input.hideInEditor = inputIsLocked;
}

void Mapping::checkFiltersNeedContinuousProcess()
{
	bool need = false;
	for (auto &f : fm.items)
	{
		if (f->needsContinuousProcess)
		{
			need = true;
			break;
		}
	}

	continuousProcess->setValue(need);
}

void Mapping::process()
{
	if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) return;
	if (input.inputReference == nullptr) return;
	if (!cdm.getIsValid(true)) return;

	Parameter * filteredParam = fm.processFilters();
	if (filteredParam == nullptr) return;
	om.setValue(filteredParam->getValue());
}

var Mapping::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("input", input.getJSONData());
	data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
	data.getDynamicObject()->setProperty("filters", fm.getJSONData());
	data.getDynamicObject()->setProperty("outputs", om.getJSONData());
	return data;
}

void Mapping::loadJSONDataInternal(var data)
{
	Processor::loadJSONDataInternal(data);
	input.loadJSONData(data.getProperty("input", var()));
	cdm.loadJSONData(data.getProperty("conditions", var()));
	fm.loadJSONData(data.getProperty("filters", var()));
	om.loadJSONData(data.getProperty("outputs", var()));

	fm.setupSource(input.inputReference);
}

void Mapping::inputReferenceChanged(MappingInput *)
{
	fm.setupSource(input.inputReference);
}

void Mapping::inputParameterValueChanged(MappingInput *)
{
	process();
}

void Mapping::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == continuousProcess)
	{
		if (continuousProcess->boolValue()) startTimerHz(30);
		else stopTimer();
	}
}

void Mapping::itemAdded(MappingFilter * m)
{
	checkFiltersNeedContinuousProcess();
}

void Mapping::itemRemoved(MappingFilter * m)
{
	checkFiltersNeedContinuousProcess();
}


ProcessorUI * Mapping::getUI()
{
	return new MappingUI(this);
}

void Mapping::timerCallback()
{
	if ((canBeDisabled && !enabled->boolValue()) || forceDisabled) return;
	process();
}
