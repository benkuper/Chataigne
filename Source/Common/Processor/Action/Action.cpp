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
    forceNoOffConsequences(false),
	hasOffConsequences(false),
    triggerOn(nullptr),
    triggerOff(nullptr),
    actionAsyncNotifier(10)
{
	helpID = "Action"; 
	itemDataType = "Action";
	type = ACTION;

	triggerOn = addTrigger("Trigger Validate", "Triggers the action");
	triggerOn->hideInEditor = true;

	cdm.setHasActivationDefinitions(params.getProperty("hasActivationDefinitions",true));
	cdm.addConditionManagerListener(this);
	cdm.addBaseManagerListener(this);
	addChildControllableContainer(&cdm);

	csmOn.reset(new ConsequenceManager("Consequences : TRUE"));

	addChildControllableContainer(csmOn.get());

	forceNoOffConsequences = params.getProperty("forceNoOffConsequences", false);

	updateConditionRoles();
}

Action::~Action()
{
}

void Action::updateConditionRoles()
{
	if (Engine::mainEngine->isClearing) return;
	
	actionRoles.clear();
	for (auto &c : cdm.items)
	{
		ActivationCondition * ac = dynamic_cast<ActivationCondition *>(c);
		if (ac != nullptr && ac->enabled->boolValue())
		{
			actionRoles.addIfNotAlreadyThere((ac->type == ActivationCondition::ON_ACTIVATE)  ? ACTIVATE : DEACTIVATE);
		}
	}

	setHasOffConsequences(!forceNoOffConsequences && !actionRoles.contains(DEACTIVATE));

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
			csmOff.reset(new ConsequenceManager("Consequences : FALSE"));
			addChildControllableContainer(csmOff.get());
		}

		if(triggerOff == nullptr)
		{
			triggerOff = addTrigger("Trigger Invalidate", "Triggers the action");
			triggerOff->hideInEditor = true;
		}

	} else
	{
		removeChildControllableContainer(csmOff.get());
		csmOff = nullptr;
		removeControllable(triggerOff);
		triggerOff = nullptr;
	}
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

	updateConditionRoles();
	if(hasOffConsequences) csmOff->loadJSONData(data.getProperty("consequencesOff", var()));

}

void Action::onContainerParameterChangedInternal(Parameter * p)
{
	Processor::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		actionListeners.call(&Action::ActionListener::actionEnableChanged, this);
		actionAsyncNotifier.addMessage(new ActionEvent(ActionEvent::ENABLED_CHANGED, this));
	}
}

void Action::onContainerTriggerTriggered(Trigger * t)
{
	Processor::onContainerTriggerTriggered(t);

	if (!enabled->boolValue() || forceDisabled) return;

	if (t == triggerOn)
	{
		csmOn->triggerAll->trigger();
	} else if (t == triggerOff)
	{
		if(hasOffConsequences) csmOff->triggerAll->trigger();
	}
}

void Action::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	Processor::controllableFeedbackUpdate(cc, c);

	ActivationCondition * ac = c->getParentAs<ActivationCondition>();
	if (ac != nullptr && c == ac->enabled)
	{
		updateConditionRoles();
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
	updateConditionRoles();
}

void Action::itemRemoved(Condition *)
{
	updateConditionRoles();
}

void Action::highlightLinkedInspectables(bool value)
{
	Processor::highlightLinkedInspectables(value);
	for (auto & cd : cdm.items) cd->highlightLinkedInspectables(value);
	if (csmOn != nullptr) for (auto & cs : csmOn->items) cs->highlightLinkedInspectables(value);
	if (csmOff != nullptr) for (auto & cs : csmOff->items) cs->highlightLinkedInspectables(value);
}

ProcessorUI * Action::getUI()
{
	return new ActionUI(this);
}
