/*
  ==============================================================================

    Mapping.cpp
    Created: 28 Oct 2016 8:06:13pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping.h"
#include "MappingEditor.h"

Mapping::Mapping(bool canBeDisabled) :
BaseItem("Mapping", canBeDisabled),
forceDisabled(false),
	inputIsLocked(false)
{
	continuousProcess = addBoolParameter("Continuous", "If enabled, the mapping will process continuously rather than only when parameter value has changed",false);

	addChildControllableContainer(&input);
	addChildIndexedControllableContainer(&cdm);
	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);


	input.addMappingInputListener(this);
}

  Mapping::~Mapping()
{
}

void Mapping::lockInputTo(Parameter * lockParam)
{
	inputIsLocked = lockParam != nullptr;
	input.lockInput(lockParam);
}

void Mapping::process()
{
	if (!enabled->boolValue() || forceDisabled) return;
	if (input.inputReference == nullptr) return;
	if (!cdm.getIsValid(true)) return;

	Parameter * filteredParam = fm.processFilters();
	if (filteredParam == nullptr) return;
	om.setValue(filteredParam->value);
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
	BaseItem::loadJSONDataInternal(data);
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
		if(continuousProcess->boolValue()) startTimerHz(30);
		else stopTimer();
	}
}

InspectableEditor * Mapping::getEditor(bool isRoot)
{
	return new MappingEditor(this, isRoot);
}

void Mapping::timerCallback()
{
	if(!enabled->boolValue() || forceDisabled) return
	process();
}
