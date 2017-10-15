/*
  ==============================================================================

    State.cpp
    Created: 28 Oct 2016 8:19:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "State.h"

State::State() :
	BaseItem("State",true,true,true),
	pm("Processors")
{
	//canInspectChildContainers = false;

	active = addBoolParameter("Active", "If active, the state's actions and mappings will be effective, otherwise this state won't do anything.", false);

	addChildControllableContainer(&pm);

	pm.setForceDisabled(true);

	//viewUISize->setPoint(200, 300);

	
}

State::~State()
{
	masterReference.clear();
}


void State::onContainerParameterChangedInternal(Parameter *p)
{
	if (p == active || p == enabled)
	{
		stateListeners.call(&StateListener::stateActivationChanged, this);
		pm.setForceDisabled(!active->boolValue() || !enabled->boolValue());
	}
}

var State::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("processors", pm.getJSONData());
	return data;
}

void State::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);	
	pm.loadJSONData(data.getProperty("processors", var()));
}
