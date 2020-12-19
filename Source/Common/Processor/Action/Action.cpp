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
#include "Condition/conditions/StandardCondition/StandardCondition.h"

Action::Action(var params) :
	Processor(getTypeString()),
    autoTriggerWhenAllConditionAreActives(true),
    forceNoOffConsequences(false),
	hasOffConsequences(false),
	iterationCount(0),
	forceChecking(false),
    actionAsyncNotifier(10)
{
	helpID = "Action"; 
	itemDataType = "Action";
	type = ACTION;

	setIterationCount(1);
	
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

void Action::setIterationCount(int count)
{
	if (iterationCount == count) return;
	iterationCount = count;

	updateTriggerSetup();

	cdm.setIterationCount(count);
}

void Action::updateTriggerSetup()
{
	while (iterationCount < triggerOns.size())
	{
		removeControllable(triggerOns[triggerOns.size() - 1]);
		triggerOns.removeLast();
	}

	while (iterationCount > triggerOns.size())
	{
		String id = String(triggerOns.size() + 1);
		Trigger* t = addTrigger("Trigger On #" + id, "Triggers the Consequence:TRUE of an action. If this is inside an iterator, this will trigger the action for the indice #" + id);
		t->hideInEditor = true;
		triggerOns.add(t);
	}


	if (hasOffConsequences)
	{
		while (iterationCount > triggerOffs.size())
		{
			removeControllable(triggerOffs[triggerOffs.size() - 1]);
			triggerOffs.removeLast();
		}

		if (hasOffConsequences)
		{
			String id = String(triggerOffs.size() + 1);
			Trigger* t = addTrigger("Trigger Off #" + id, "Triggers the Consequence:FALSE of an action. If this is inside an iterator, this will trigger the action for the indice #" + id);
			t->hideInEditor = true;
			triggerOffs.add(t);
		}
	}
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
	} else
	{
		removeChildControllableContainer(csmOff.get());
		csmOff = nullptr;
	}

	updateTriggerSetup();
}

void Action::updateDisables(bool force)
{
	Processor::updateDisables();
	cdm.setForceDisabled(forceDisabled || !enabled->boolValue(), force);
	csmOn->setForceDisabled(forceDisabled || !enabled->boolValue(), force);
	if (hasOffConsequences) csmOff->setForceDisabled(forceDisabled || !enabled->boolValue(), force);
}

void Action::forceCheck(bool triggerIfChanged)
{
	if (!triggerIfChanged) forceChecking = true;
	cdm.forceCheck();
	forceChecking = false;
}

var Action::getJSONData()
{
	var data = Processor::getJSONData();
	data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
	data.getDynamicObject()->setProperty("consequences", csmOn->getJSONData());
	if(hasOffConsequences) data.getDynamicObject()->setProperty("consequencesOff", csmOff->getJSONData());
	return data;
}

void Action::loadJSONDataItemInternal(var data)
{
	Processor::loadJSONDataItemInternal(data);

	cdm.loadJSONData(data.getProperty("conditions", var()));
	csmOn->loadJSONData(data.getProperty("consequences", var()));

	updateConditionRoles();

	if (enabled->boolValue() && !forceDisabled && actionRoles.contains(Role::ACTIVATE) && Engine::mainEngine->isLoadingFile) Engine::mainEngine->addEngineListener(this);

	if(hasOffConsequences) csmOff->loadJSONData(data.getProperty("consequencesOff", var()));
}

void Action::endLoadFile()
{
	Engine::mainEngine->removeEngineListener(this);
	if (actionRoles.contains(Role::ACTIVATE))
	{
		for (int i = 0; i < iterationCount; i++)
		{
			if (cdm.getIsValid(i, false)) triggerOns[i]->trigger();
		}
	}
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

	if (!forceChecking)
	{
		if (triggerOns.contains(t))
		{
			csmOn->triggerAlls[triggerOns.indexOf(t)]->trigger();
		}
		else if (triggerOffs.contains(t))
		{
			if (hasOffConsequences) csmOff->triggerAlls[triggerOffs.indexOf(t)]->trigger();
		}
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

void Action::conditionManagerValidationChanged(ConditionManager *, const IterativeContext &context)
{
	if (autoTriggerWhenAllConditionAreActives)
	{
		if (cdm.isValids[context.indexInList]->boolValue())
		{
			if(context.indexInList < triggerOns.size()) triggerOns[context.indexInList]->trigger(); //force trigger from onContainerTriggerTriggered, for derivating child classes
		} else
		{
			if (hasOffConsequences)
			{
				if (context.indexInList < triggerOffs.size()) triggerOffs[context.indexInList]->trigger();
			}
		}
	}

	actionListeners.call(&ActionListener::actionValidationChanged, this);
	actionAsyncNotifier.addMessage(new ActionEvent(ActionEvent::ActionEvent::VALIDATION_CHANGED, this));
}


void Action::itemAdded(Condition * c)
{
	updateConditionRoles();

	StandardCondition* sc = dynamic_cast<StandardCondition*>(c);
	if (sc != nullptr) sc->sourceTarget->warningResolveInspectable = this;
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