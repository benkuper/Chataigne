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
	viewUIPosition = addPoint2DParameter("Position", "Position in State Machine view");
	viewUIPosition->setBounds(-100, -100,100,100);
	viewUIPosition->hideInEditor = true;
	viewUIPosition->hideInOutliner = true;
	viewUIPosition->isTargettable = false;

	viewUISize = addPoint2DParameter("Size", "Size in State Machine view");
	viewUISize->setBounds(30,60,500,500);
	viewUISize->setPoint(200, 250);
	viewUISize->hideInEditor = true;
	viewUISize->hideInOutliner = true;
	viewUISize->isTargettable = false;

	miniMode = addBoolParameter("Mini Mode", "Set the mini mode",false);
	miniMode->hideInOutliner = true;
	miniMode->hideInEditor = true;
	miniMode->isTargettable = false;

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
