/*
  ==============================================================================

    TimeTrigger.cpp
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "TimeTrigger.h"


TimeTrigger::TimeTrigger(float _time, float flagYPos, const String &name) :
	Action(name)
{
	itemDataType = "TimeTrigger"; 
	
	time = addFloatParameter("Time", "Time at which the action is triggered", 0, 0, 3600);
	
	time->defaultUI = FloatParameter::TIME;
	flagY = addFloatParameter("Flag Y", "Position of the trigger's flag", 0,0,1);
	isTriggered = addBoolParameter("Is Triggered", "Is this Time Trigger already triggered during this playing ?", false);
	
	time->setValue(_time);
	flagY->setValue(flagYPos);

	isLocked = addBoolParameter("Locked", "When locked, you can't change time or flag values", false);

	isTriggered->hideInEditor = true;
	isTriggered->isSavable = false;

	autoTriggerWhenAllConditionAreActives = false;
}

TimeTrigger::~TimeTrigger()
{

}

void TimeTrigger::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == isLocked)
	{
		time->setControllableFeedbackOnly(isLocked->boolValue());
		flagY->setControllableFeedbackOnly(isLocked->boolValue());
	}
}

void TimeTrigger::trigger()
{
	isTriggered->setValue(true);
	triggerOn->trigger();
}