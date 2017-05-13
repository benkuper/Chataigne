/*
  ==============================================================================

    State.cpp
    Created: 28 Oct 2016 8:19:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "State.h"

State::State() :
	BaseItem("State",true,true,true)
{
	canInspectChildContainers = false;

	active = addBoolParameter("Active", "If active, the state's actions and mappings will be effective, otherwise this state won't do anything.", false);
	permanent = addBoolParameter("Permanent", "If permanent, the state won't be affected by the state machine logic, i.e. deactivating when another state is activated.",false);

	addChildControllableContainer(&am);
	addChildControllableContainer(&mm);

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
		am.setForceDisabled(!active->boolValue() || !enabled->boolValue());
		mm.setForceDisabled(!active->boolValue() || !enabled->boolValue());
	}
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
