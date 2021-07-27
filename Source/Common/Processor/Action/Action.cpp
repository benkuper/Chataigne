/*
  ==============================================================================

	Action.cpp
	Created: 28 Oct 2016 8:05:02pm
	Author:  bkupe

  ==============================================================================
*/

Action::Action(var params, Multiplex * multiplex) :
	Processor(getTypeString()),
	MultiplexTarget(multiplex),
    autoTriggerWhenAllConditionAreActives(true),
    forceNoOffConsequences(false),
	hasOffConsequences(false),
	cdm(multiplex),
    triggerOn(nullptr),
    forceChecking(false),
    actionAsyncNotifier(10)
{
	helpID = "Action"; 
	itemDataType = "Action";
	type = ACTION;

	triggerOn = addTrigger("Trigger", "This will trigger as if the conditions have been validated, and trigger all the Consequences:TRUE");
	triggerOn->hideInEditor = true;

	cdm.setHasActivationDefinitions(params.getProperty("hasActivationDefinitions",true));
	cdm.addConditionManagerListener(this);
	cdm.addBaseManagerListener(this);
	addChildControllableContainer(&cdm);

	csmOn.reset(new ConsequenceManager("Consequences : TRUE", multiplex));

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


void Action::setHasOffConsequences(bool value)
{
	if (Engine::mainEngine->isClearing) return;
	if (value == hasOffConsequences) return;

	hasOffConsequences = value;

	if (hasOffConsequences)
	{
		if (csmOff == nullptr)
		{
			csmOff.reset(new ConsequenceManager("Consequences : FALSE", multiplex));
			addChildControllableContainer(csmOff.get());
		}
	} else
	{
		removeChildControllableContainer(csmOff.get());
		csmOff = nullptr;
	}
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


void Action::triggerConsequences(bool triggerTrue, int multiplexIndex)
{
	if (!enabled->boolValue() || forceDisabled) return;

	if (!forceChecking)
	{
		if (triggerTrue) csmOn->triggerAll(multiplexIndex);
		else csmOff->triggerAll(multiplexIndex);
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
		for (int i = 0; i < getMultiplexCount(); i++) if (cdm.getIsValid(i, false)) triggerConsequences(true, i);
	}
}

void Action::onContainerTriggerTriggered(Trigger* t)
{
	Processor::onContainerTriggerTriggered(t);

	if (t == triggerOn)
	{
		if (enabled->boolValue())
		{
			for (int i = 0; i < getMultiplexCount(); i++) triggerConsequences(true, i);
		}
	}
}


void Action::onContainerParameterChangedInternal(Parameter * p)
{
	Processor::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		if(triggerOn != nullptr) triggerOn->setEnabled(enabled->boolValue());
		
		actionListeners.call(&Action::ActionListener::actionEnableChanged, this);
		actionAsyncNotifier.addMessage(new ActionEvent(ActionEvent::ENABLED_CHANGED, this));
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

void Action::conditionManagerValidationChanged(ConditionManager *, int multiplexIndex, bool dispatchOnChangeOnly)
{
	if (forceChecking) return;

	if (autoTriggerWhenAllConditionAreActives)
	{
		if (cdm.getIsValid(multiplexIndex, false)) triggerConsequences(true, multiplexIndex); //force trigger from onContainerTriggerTriggered, for derivating child classes
		else if (hasOffConsequences) triggerConsequences(false, multiplexIndex);
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


void Action::multiplexPreviewIndexChanged()
{
	actionAsyncNotifier.addMessage(new ActionEvent(ActionEvent::ActionEvent::MULTIPLEX_PREVIEW_CHANGED, this));
}


ProcessorUI * Action::getUI()
{
	return new ActionUI(this);
}
