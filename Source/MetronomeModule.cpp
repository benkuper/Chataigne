/*
  ==============================================================================

    MetronomeModule.cpp
    Created: 30 Dec 2017 10:09:22pm
    Author:  Ben

  ==============================================================================
*/

#include "MetronomeModule.h"

MetronomeModule::MetronomeModule() :
	Module(getTypeString())
{
	setupIOConfiguration(false, false);

	frequency = addFloatParameter("Frequency", "Frequency of the timer, in Hz (the greater the value, the faster the tempo)", 1, 0.0001f, 100);
	onTime = addFloatParameter("ON Time", "Amount of time the metronome stays valid when triggered", .1f, 0, 1);

	tick = valuesCC.addBoolParameter("Tick", "When the metronome is ticking", false);
	startTimer(0, 1000.0f / frequency->floatValue());
}

MetronomeModule::~MetronomeModule()
{
}

void MetronomeModule::onContainerParameterChangedInternal(Parameter * p)
{
	if (p == frequency)
	{
		if (isTimerRunning(0)) stopTimer(0);
		startTimer(0, 1000.0f / frequency->floatValue());
	}
}

void MetronomeModule::timerCallback(int timerID)
{
	if (!enabled->boolValue()) return;

	if (timerID == 0)
	{
		tick->setValue(true);
		startTimer(1, onTime->floatValue() * 1000);
	} else if (timerID == 1)
	{
		tick->setValue(false);
		stopTimer(1);
	}
}
