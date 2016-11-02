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
	valuesCC("Values")
{
	recursiveInspectionLevel = 1;
	addChildControllableContainer(&valuesCC);

	logIncomingData = addBoolParameter("Log Incoming Data", "Enable / Disable logging of incoming data for this input", false);
	logIncomingData->hideInOutliner = true;

	activityTrigger = addTrigger("Activity", "Activity Signal");
	activityTrigger->hideInEditor = true;
}

Input::~Input()
{
	
}

Array<WeakReference<Controllable>> Input::getValueControllables()
{
	return valuesCC.getAllControllables();
}

var Input::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	return data;
}
