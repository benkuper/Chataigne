/*
  ==============================================================================

    Action.cpp
    Created: 28 Oct 2016 8:05:02pm
    Author:  bkupe

  ==============================================================================
*/

#include "Action.h"
#include "ActionEditor.h"

Action::Action(const String & name) :
	BaseItem(name),
	autoTriggerWhenAllConditionAreActives(true),
	forceDisabled(false)
{
	addChildControllableContainer(&cdm);
	addChildControllableContainer(&csm);

	cdm.addConditionManagerListener(this);

	isValid = addBoolParameter("Is Valid", "Are all condition valids ?", false);
	isValid->hideInEditor = true;
	trigger = addTrigger("Trigger", "Triggers the action");
}

  Action::~Action()
{
}

var Action::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("conditions", cdm.getJSONData());
	data.getDynamicObject()->setProperty("consequences", csm.getJSONData());
	return data;
}

void Action::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	cdm.loadJSONData(data.getProperty("conditions", var()));
	csm.loadJSONData(data.getProperty("consequences", var()));
}

void Action::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == enabled) actionListeners.call(&Action::ActionListener::actionEnableChanged,this);
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
	DBG("Condition validation changed");
	if (cdm.isValid->boolValue())
	{
		csm.triggerAll->trigger();
	}
}

InspectableEditor * Action::getEditor(bool isRoot)
{
	return new ActionEditor(this,isRoot);
}
