/*
  ==============================================================================

	Action.cpp
	Created: 28 Oct 2016 8:05:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Action.h"
#include "ActionUI.h"

Action::Action(const String & name, var params) :
	Processor(name),
	autoTriggerWhenAllConditionAreActives(true),
	triggerOn(nullptr),
	triggerOff(nullptr)
{
	type = ACTION;

	addChildControllableContainer(&cdm);

	csmOn = new ConsequenceManager("Consequences : TRUE");
	addChildControllableContainer(csmOn);

	hasOffConsequences = params.getProperty("hasOffConsequences", false);

	if (hasOffConsequences)
	{
		csmOff = new ConsequenceManager("Consequences : FALSE");
		addChildControllableContainer(csmOff);
	}
	
	cdm.addConditionManagerListener(this);

	triggerOn = addTrigger("Trigger Validate", "Triggers the action");
	triggerOn->hideInEditor = true;

	if (hasOffConsequences)
	{
		triggerOff = addTrigger("Trigger Invalidate", "Triggers the action");
		triggerOff->hideInEditor = true;
	}
	

	
	helpID = "Action";
}

Action::~Action()
{
}

void Action::setForceDisabled(bool value, bool force)
{
	Processor::setForceDisabled(value, force);
	cdm.setForceDisabled(value);
	csmOn->setForceDisabled(value);
	csmOff->setForceDisabled(value);
}

var Action::getJSONData()
{
	var data = Processor::getJSONData();
	data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
	data.getDynamicObject()->setProperty("consequences", csmOn->getJSONData());
	if(hasOffConsequences) data.getDynamicObject()->setProperty("consequencesOff", csmOff->getJSONData());
	return data;
}

void Action::loadJSONDataInternal(var data)
{
	Processor::loadJSONDataInternal(data);
	cdm.loadJSONData(data.getProperty("conditions", var()));
	csmOn->loadJSONData(data.getProperty("consequences", var()));
	if(hasOffConsequences) csmOff->loadJSONData(data.getProperty("consequencesOff", var()));
}

void Action::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled) actionListeners.call(&Action::ActionListener::actionEnableChanged, this);
}

void Action::onContainerTriggerTriggered(Trigger * t)
{
	if (!enabled->boolValue() || forceDisabled) return;

	if (t == triggerOn)
	{
		csmOn->triggerAll->trigger();
	} else if (t == triggerOff)
	{
		if(hasOffConsequences) csmOff->triggerAll->trigger();
	}
}

void Action::conditionManagerValidationChanged(ConditionManager *)
{
	if (cdm.isValid->boolValue())
	{
		triggerOn->trigger(); //force trigger from onContainerTriggerTriggered, for derivating child classes
	} else
	{
		if(hasOffConsequences) triggerOff->trigger();
	}
}


ProcessorUI * Action::getUI()
{
	return new ActionUI(this);
}
