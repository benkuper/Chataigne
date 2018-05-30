/*
  ==============================================================================

	Action.cpp
	Created: 28 Oct 2016 8:05:02pm
	Author:  bkupe

  ==============================================================================
*/

#include "Action.h"
#include "ui/ActionUI.h"
#include "Condition/conditions/ActivationCondition/ActivationCondition.h"

Action::Action(const String & name, var params) :
	Processor(params.getProperty("name", name)),
	autoTriggerWhenAllConditionAreActives(true),
	actionRole(Action::STANDARD),
	triggerOn(nullptr),
	triggerOff(nullptr),
	forceNoOffConsequences(false),
	hasOffConsequences(false),
	actionAsyncNotifier(10)
{
	itemDataType = "Action";
	type = ACTION;

	triggerOn = addTrigger("Trigger Validate", "Triggers the action");
	triggerOn->hideInEditor = true;

	cdm.setHasActivationDefinitions(params.getProperty("hasActivationDefinitions",true));
	cdm.addConditionManagerListener(this);
	cdm.addBaseManagerListener(this);
	addChildControllableContainer(&cdm);

	csmOn = new ConsequenceManager("Consequences : TRUE");
	addChildControllableContainer(csmOn);

	forceNoOffConsequences = params.getProperty("forceNoOffConsequences", false);
	setRole((Role)(int)params.getProperty("role", STANDARD), true);

	helpID = "Action";
}

Action::~Action()
{
}

void Action::updateConditionRole()
{
	Role r = STANDARD;
	for (auto &c : cdm.items)
	{
		ActivationCondition * ac = dynamic_cast<ActivationCondition *>(c);
		if (ac != nullptr)
		{
			r = ac->type == ActivationCondition::ON_ACTIVATE ? ACTIVATE : DEACTIVATE;
			break;
		}
	}

	setRole(r);
}

void Action::setRole(Role role, bool force)
{
	if (Engine::mainEngine->isClearing) return;
	if (actionRole == role && !force) return;

	actionRole = role;
	setHasOffConsequences(!forceNoOffConsequences && actionRole == STANDARD);
	
	actionListeners.call(&ActionListener::actionRoleChanged, this);
	actionAsyncNotifier.addMessage(new ActionEvent(ActionEvent::ROLE_CHANGED, this));

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
	if (Engine::mainEngine->isClearing) return;
	if (value == hasOffConsequences) return;

	hasOffConsequences = value;

	if (hasOffConsequences)
	{
		if (csmOff == nullptr)
		{
			csmOff = new ConsequenceManager("Consequences : FALSE");
			addChildControllableContainer(csmOff);
		}

		if(triggerOff == nullptr)
		{
			triggerOff = addTrigger("Trigger Invalidate", "Triggers the action");
			triggerOff->hideInEditor = true;
		}

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

	cdm.loadJSONData(data.getProperty("conditions", var()));
	csmOn->loadJSONData(data.getProperty("consequences", var()));

	updateConditionRole();
	if(hasOffConsequences) csmOff->loadJSONData(data.getProperty("consequencesOff", var()));

}

void Action::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled)
	{
		actionListeners.call(&Action::ActionListener::actionEnableChanged, this);
		actionAsyncNotifier.addMessage(new ActionEvent(ActionEvent::ENABLED_CHANGED, this));
	}
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

	actionListeners.call(&ActionListener::actionValidationChanged, this);
	actionAsyncNotifier.addMessage(new ActionEvent(ActionEvent::ActionEvent::VALIDATION_CHANGED, this));
}


void Action::itemAdded(Condition *)
{
	updateConditionRole();
}

void Action::itemRemoved(Condition *)
{
	updateConditionRole();
}

ProcessorUI * Action::getUI()
{
	return new ActionUI(this);
}
