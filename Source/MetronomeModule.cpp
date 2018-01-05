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

	frequency = moduleParams.addFloatParameter("Frequency", "Frequency of the timer, in Hz (the greater the value, the faster the tempo)", 1, 0.0001f, 100);
	onTime = moduleParams.addFloatParameter("ON Time", "Amount of time the metronome stays valid when triggered", .5f, 0, 1);
	random = moduleParams.addFloatParameter("Randomness", "Amount of randomness in each call", 0, 0, 1);

	tick = valuesCC.addBoolParameter("Tick", "When the metronome is ticking", false);
	startTimer(0, 1000.0f / frequency->floatValue());

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = false;
}

MetronomeModule::~MetronomeModule()
{
}

void MetronomeModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == frequency)
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
		float nextTime = 1000 / frequency->floatValue();
		
		if (random->floatValue() > 0)
		{
			nextTime = (1 - rnd.nextFloat()*random->floatValue())*nextTime;
			startTimer(0, nextTime);
		}

		startTimer(1, onTime->floatValue() * nextTime);
	} else if (timerID == 1)
	{
		tick->setValue(false);
		stopTimer(1);
	}
}
