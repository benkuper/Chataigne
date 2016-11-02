/*
  ==============================================================================

    Action.cpp
    Created: 28 Oct 2016 8:05:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "Action.h"
#include "ActionEditor.h"

Action::Action() :
	BaseItem("Action")
{
	saveAndLoadRecursiveData = false;

	addChildControllableContainer(&cdm);
	addChildControllableContainer(&csm);
}

Action::~Action()
{
}

var Action::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
	data.getDynamicObject()->setProperty("consequences", csm.getJSONData());
	return data;
}

void Action::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	cdm.loadJSONData(data.getProperty("conditions", var()));
	csm.loadJSONData(data.getProperty("consequences", var()));
}

void Action::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled) actionListeners.call(&Action::ActionListener::actionEnableChanged,this);
}

InspectorEditor * Action::getEditor()
{
	return new ActionEditor(this);
}
