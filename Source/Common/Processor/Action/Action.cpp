/*
  ==============================================================================

	Action.cpp
	Created: 28 Oct 2016 8:05:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Action.h"
#include "ui/ActionUI.h"

Action::Action(const String & name, var params) :
	Processor(params.getProperty("name", name)),
	autoTriggerWhenAllConditionAreActives(true),
	actionRole(Action::STANDARD),
	triggerOn(nullptr),
	triggerOff(nullptr),
	hasOffConsequences(false)
{
	type = ACTION;
	actionRole = (Role)(int)params.getProperty("role", STANDARD);

	addChildControllableContainer(&cdm);

	csmOn = new ConsequenceManager("Consequences : TRUE");
	addChildControllableContainer(csmOn);

	setHasOffConsequences(params.getProperty("hasOffConsequences", false) && actionRole == STANDARD);

	cdm.addConditionManagerListener(this);

	triggerOn = addTrigger("Trigger Validate", "Triggers the action");
	triggerOn->hideInEditor = true;

	
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
	if(hasOffConsequences) csmOff->setForceDisabled(value);
}

void Action::setHasOffConsequences(bool value)
{
	if (value == hasOffConsequences) return;
	hasOffConsequences = value;
	if (hasOffConsequences)
	{
		csmOff = new ConsequenceManager("Consequences : FALSE");
		addChildControllableContainer(csmOff);
		triggerOff = addTrigger("Trigger Invalidate", "Triggers the action");
		triggerOff->hideInEditor = true;

	} else
	{
		removeChildControllableContainer(csmOff);
		csmOff = nullptr;
		removeControllable(triggerOff);
		triggerOff = nullptr;
	}
}

var Action::getJSONData()
{
	var data = Processor::getJSONData();
	if(actionRole != STANDARD) data.getDynamicObject()->setProperty("role", actionRole);
	data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
	data.getDynamicObject()->setProperty("consequences", csmOn->getJSONData());
	if(hasOffConsequences) data.getDynamicObject()->setProperty("consequencesOff", csmOff->getJSONData());
	return data;
}

void Action::loadJSONDataInternal(var data)
{
	Processor::loadJSONDataInternal(data);
	actionRole = (Role)(int)data.getProperty("role", actionRole);
	if (actionRole != STANDARD) setHasOffConsequences(false);

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
	if (autoTriggerWhenAllConditionAreActives)
	{
		if (cdm.isValid->boolValue())
		{
			triggerOn->trigger(); //force trigger from onContainerTriggerTriggered, for derivating child classes
		} else
		{
			if (hasOffConsequences) triggerOff->trigger();
		}
	}
}


ProcessorUI * Action::getUI()
{
	return new ActionUI(this);
}
