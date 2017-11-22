/*
  ==============================================================================

	Action.cpp
	Created: 28 Oct 2016 8:05:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Action.h"
#include "ActionEditor.h"
#include "ActionUI.h"

Action::Action(const String & name) :
	Processor(name),
	autoTriggerWhenAllConditionAreActives(true)
{
	type = ACTION;

	addChildControllableContainer(&cdm);
	addChildControllableContainer(&csm);

	cdm.addConditionManagerListener(this);

	isValid = addBoolParameter("Is Valid", "Are all condition valids ?", false);
	isValid->hideInEditor = true;
	trigger = addTrigger("Trigger", "Triggers the action");

	helpID = "Action";
}

Action::~Action()
{
}

void Action::setForceDisabled(bool value)
{
	Processor::setForceDisabled(value);
	csm.setForceDisabled(value);
}

var Action::getJSONData()
{
	var data = Processor::getJSONData();
	data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
	data.getDynamicObject()->setProperty("consequences", csm.getJSONData());
	return data;
}

void Action::loadJSONDataInternal(var data)
{
	Processor::loadJSONDataInternal(data);
	cdm.loadJSONData(data.getProperty("conditions", var()));
	csm.loadJSONData(data.getProperty("consequences", var()));
}

void Action::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled) actionListeners.call(&Action::ActionListener::actionEnableChanged, this);
}

void Action::onContainerTriggerTriggered(Trigger * t)
{
	if (!enabled->boolValue() || forceDisabled) return;

	if (t == trigger)
	{
		csm.triggerAll->trigger();
	}
}

void Action::conditionManagerValidationChanged(ConditionManager *)
{
	if (cdm.isValid->boolValue())
	{
		trigger->trigger(); //force trigger from onContainerTriggerTriggered, for derivating child classes
	}
}

InspectableEditor * Action::getEditor(bool isRoot)
{
	return new ActionEditor(this, isRoot);
}

ProcessorUI * Action::getUI()
{
	return new ActionUI(this);
}
