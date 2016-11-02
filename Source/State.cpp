/*
  ==============================================================================

    State.cpp
    Created: 28 Oct 2016 8:19:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "State.h"

State::State() :
	BaseItem("State")
{
	saveAndLoadRecursiveData = false; //custom inner data

	viewUIPosition = addPoint2DParameter("Position", "Position in State Machine view");
	viewUIPosition->setBounds(-100, -100,100,100);
	viewUIPosition->hideInOutliner = true;

	miniMode = addBoolParameter("Mini Mode", "Set the mini mode",false);
	miniMode->hideInOutliner = true;
	miniMode->hideInEditor = true;

	addChildControllableContainer(&am);
	addChildControllableContainer(&mm);
}

State::~State()
{
}

var State::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("actions", am.getJSONData());
	data.getDynamicObject()->setProperty("mappings", mm.getJSONData());
	return data;
}

void State::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	am.loadJSONData(data.getProperty("actions", var()));
	mm.loadJSONData(data.getProperty("mappings", var()));
}
