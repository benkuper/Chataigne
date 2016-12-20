/*
  ==============================================================================

    Mapping.cpp
    Created: 28 Oct 2016 8:06:13pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping.h"

Mapping::Mapping() :
	BaseItem("Mapping"),
	inputIsLocked(false)
{
	canInspectChildContainers = true;

	saveAndLoadRecursiveData = false;
	addChildControllableContainer(&input);
	addChildControllableContainer(&fm);
	addChildControllableContainer(&om);
}

Mapping::~Mapping()
{
}

void Mapping::lockInputTo(Parameter * lockParam)
{
	inputIsLocked = lockParam != nullptr;
	if (inputIsLocked) input.lockInput(lockParam);
	
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
}
