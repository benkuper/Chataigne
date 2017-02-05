/*
  ==============================================================================

    Mapping.cpp
    Created: 28 Oct 2016 8:06:13pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping.h"
#include "MappingEditor.h"

Mapping::Mapping() :
	BaseItem("Mapping"),
	inputIsLocked(false)
{
	canInspectChildContainers = true;
	addChildControllableContainer(&input);
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
	if (inputIsLocked) input.lockInput(lockParam);
	
}

void Mapping::process()
{
	if (input.inputReference == nullptr) return;

	Parameter * filteredParam = fm.processFilters();
	om.setValue(filteredParam->value);
}

var Mapping::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("input", input.getJSONData());
	data.getDynamicObject()->setProperty("filters", fm.getJSONData());
	data.getDynamicObject()->setProperty("outputs", om.getJSONData());
	return data;
}

void Mapping::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	input.loadJSONData(data.getProperty("input", var()));
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
	if (!enabled->boolValue()) return;
	process();
}

InspectableEditor * Mapping::getEditor(bool isRoot)
{
	return new MappingEditor(this, isRoot);
}
