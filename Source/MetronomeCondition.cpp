/*
  ==============================================================================

    MetronomeCondition.cpp
    Created: 19 Nov 2017 11:14:55pm
    Author:  Ben

  ==============================================================================
*/

#include "MetronomeCondition.h"
#include "MetronomeConditionEditor.h"

MetronomeCondition::MetronomeCondition()
{
	frequency = addFloatParameter("Frequency", "Frequency of the timer, in Hz (the greater the value, the faster the tempo)", 1, 0.0001f, 100);
	onTime = addFloatParameter("ON Time", "Amount of time the metronome stays valid when triggered", .1f, 0, 1);

	startTimer(0, 1000.0f / frequency->floatValue());
}

MetronomeCondition::~MetronomeCondition()
{
}


void MetronomeCondition::onContainerParameterChangedInternal(Parameter * p)
{
	Condition::onContainerParameterChangedInternal(p);
	if (p == frequency)
	{
		if(isTimerRunning(0)) stopTimer(0);
		startTimer(0, 1000.0f / frequency->floatValue());
	}
}

void MetronomeCondition::timerCallback(int timerID)
{
	if (!enabled->boolValue()) return;

	if (timerID == 0)
	{
		isValid->setValue(true);
		startTimer(1, onTime->floatValue() * 1000);
	}
	else if (timerID == 1)
	{
		isValid->setValue(false); 
		stopTimer(1);
	}
}


InspectableEditor * MetronomeCondition::getEditor(bool isRoot)
{
	return new MetronomeConditionEditor(this, isRoot);
}
