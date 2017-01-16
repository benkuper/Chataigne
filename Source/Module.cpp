/*
  ==============================================================================

    Module.cpp
    Created: 8 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#include "Module.h"
#include "CommandFactory.h"
#include "ModuleEditor.h"

Module::Module(const String &name) :
	BaseItem(name),
	valuesCC("Values")
{
	canInspectChildContainers = true;

	addChildControllableContainer(&valuesCC);


	logIncomingData = addBoolParameter("Log Incoming", "Enable / Disable logging of incoming data for this module", false);
	logIncomingData->hideInOutliner = true;
	logIncomingData->isTargettable = false;

	logOutgoingData = addBoolParameter("Log Outgoing", "Enable / Disable logging of outgoing data for this module", false);
	logOutgoingData->hideInOutliner = true;
	logOutgoingData->isTargettable = false;


	inActivityTrigger = addTrigger("IN Activity", "Incoming Activity Signal");
	inActivityTrigger->hideInEditor = true;

	outActivityTrigger = addTrigger("OUT Activity", "Outgoing Activity Signal");
	outActivityTrigger->hideInEditor = true;
}

Module::~Module()
{

}

Array<WeakReference<Controllable>> Module::getValueControllables()
{
	return valuesCC.getAllControllables();
}


var Module::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("type", getTypeString());
	return data;
}

/*
InspectableEditor * Module::getEditor(bool isRoot)
{
	return new ModuleEditor(this, isRoot);
}
*/
