/*
  ==============================================================================

    Input.cpp
    Created: 28 Oct 2016 8:02:24pm
    Author:  bkupe

  ==============================================================================
*/

#include "Input.h"

Input::Input(const String &name) :
	BaseItem(name),
	valueContainer("Values")
{
	recursiveInspectionLevel = 1;
	addChildControllableContainer(&valueContainer);
}

Input::~Input()
{

}

Array<WeakReference<Controllable>> Input::getValueControllables()
{
	return valueContainer.getAllControllables();
}

var Input::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	return data;
}
