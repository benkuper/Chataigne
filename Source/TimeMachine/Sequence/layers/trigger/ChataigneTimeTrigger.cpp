/*
  ==============================================================================

    TimeTrigger.cpp
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "Common/Processor/ProcessorIncludes.h"

ChataigneTimeTrigger::ChataigneTimeTrigger(StringRef name) :
	alreadyTriggered(false),
	TimeTrigger(name)
{ 
	triggerCondition = addEnumParameter("Evaluate conditions", "Where to check the condition, and wether the consequences can be triggered again more than one time");
	triggerCondition->addOption("Only once", onlyOnce);
	triggerCondition->addOption("Only on enter and exit", onlyEnterExit);
	triggerCondition->addOption("Always", always);

	onValidExit = addEnumParameter("On exit (active)", "Action when exiting the Trigger if the trigger is active");
	onValidExit->addOption("Deactivate - Trigger FALSE", action_triggerFalse);
	onValidExit->addOption("Nothing", action_nothing);
	onValidExit->addOption("Deactivate - No Trigger", action_deactivate);

	onInvalidExit = addEnumParameter("On exit (inactive)", "Action when exiting the Trigger if the trigger is inactive or the condition is false");
	onInvalidExit->addOption("Nothing", action_nothing);
	onInvalidExit->addOption("Deactivate - No Trigger", action_deactivate);
	onInvalidExit->addOption("Deactivate - Trigger TRUE", action_triggerTrue);
	onInvalidExit->addOption("Deactivate - Trigger FALSE", action_triggerFalse);
	onInvalidExit->addOption("Deactivate - Trigger both", action_triggerBoth);

	onRewind = addEnumParameter("On rewind", "Action when jumping back in the sequence before the trigger");
	onRewind->addOption("Deactivate - Trigger FALSE", action_triggerFalse);
	onRewind->addOption("Deactivate - No Trigger", action_deactivate);

	cdm.reset(new ConditionManager(nullptr));
	addChildControllableContainer(cdm.get());
	cdm->addConditionManagerListener(this);

	csm.reset(new ConsequenceManager("Consequences : TRUE"));
	untcsm.reset(new ConsequenceManager("Consequences : FALSE"));
	addChildControllableContainer(csm.get());
	addChildControllableContainer(untcsm.get());

	triggerAtAnyTime = true;

	updateTriggerParams();
}

ChataigneTimeTrigger::~ChataigneTimeTrigger()
{
}

void ChataigneTimeTrigger::updateTriggerParams()
{
	bool longTrigger = length->floatValue() > 0.f;
	triggerCondition->hideInEditor = !longTrigger;
	onValidExit->hideInEditor = !longTrigger;
	onInvalidExit->hideInEditor = !longTrigger;

	if (longTrigger)
	{
		triggerAtAnyTime = triggerCondition->getValueDataAsEnum<evaluateSetting>() != onlyEnterExit;
		bool ct = cdm->getIsValid(0, true) && (triggerCondition->getValueDataAsEnum<evaluateSetting>() != onlyOnce || !alreadyTriggered);
		canTrigger->setValue(ct);
	}
	else
	{
		triggerAtAnyTime = false;
		alreadyTriggered = false;
		canTrigger->setValue(cdm->getIsValid(0, true));
	}
}

void ChataigneTimeTrigger::onContainerParameterChangedInternal(Parameter* p)
{
	TimeTrigger::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		csm->setForceDisabled(!enabled->boolValue());
		untcsm->setForceDisabled(!enabled->boolValue());
	}
	else if(p == triggerCondition || p == length)
	{
		updateTriggerParams();
	}
}

void ChataigneTimeTrigger::conditionManagerValidationChanged(ConditionManager*, int multiplexIndex, bool dispatchOnChangeOnly)
{
	updateTriggerParams();

	if(collisionState && triggerAtAnyTime && !cdm->getIsValid(0, true))
	{
		unTrigger();
	}
}

void ChataigneTimeTrigger::triggerInternal()
{
	csm->triggerAll();
}

void ChataigneTimeTrigger::unTriggerInternal()
{
	untcsm->triggerAll();
	alreadyTriggered = true;
	updateTriggerParams();
}

void ChataigneTimeTrigger::exitedInternal(bool rewind)
{
	bool exitCondition = triggerAtAnyTime ? isTriggered->boolValue() : cdm->getIsValid(0, true);

	if(rewind)
	{
		switch (onRewind->getValueDataAsEnum<possibleActions>())
		{
		case action_deactivate:
			isTriggered->setValue(false);
			break;

		case action_triggerFalse:
			unTrigger();
			break;
		
		default:
			break;
		}
	}
	else if(exitCondition)
	{
		switch (onValidExit->getValueDataAsEnum<possibleActions>())
		{
		case action_deactivate:
			isTriggered->setValue(false);
			break;

		case action_triggerFalse:
			unTrigger();
			break;
		
		default:
			break;
		}
	}
	else
	{
		enum possibleActions param = onInvalidExit->getValueDataAsEnum<possibleActions>();
		if(param == action_triggerTrue || param == action_triggerBoth)
		{
			csm->triggerAll();
		}
		
		if(param == action_triggerFalse || param == action_triggerBoth)
		{
			untcsm->triggerAll();
		}
	}

	alreadyTriggered = false;
	updateTriggerParams();
}

var ChataigneTimeTrigger::getJSONData(bool includeNonOverriden)
{
	var data = TimeTrigger::getJSONData(includeNonOverriden);
	data.getDynamicObject()->setProperty("consequences", csm->getJSONData());
	data.getDynamicObject()->setProperty("untriggerConsequences", untcsm->getJSONData());
	data.getDynamicObject()->setProperty("conditions", cdm->getJSONData());
	return data;
}

void ChataigneTimeTrigger::loadJSONDataInternal(var data)
{
	TimeTrigger::loadJSONDataInternal(data);
	csm->loadJSONData(data.getProperty("consequences", var()));
	untcsm->loadJSONData(data.getProperty("untriggerConsequences", var()));
	cdm->loadJSONData(data.getProperty("conditions", var()));
}
