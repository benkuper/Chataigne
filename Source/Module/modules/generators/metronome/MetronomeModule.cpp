/*
  ==============================================================================

    MetronomeModule.cpp
    Created: 30 Dec 2017 10:09:22pm
    Author:  Ben

  ==============================================================================
*/
#include "Module/ModuleIncludes.h"

MetronomeModule::MetronomeModule() :
	Module(getTypeString()),
	Thread("Metronome"),
	freqTimeBpm(nullptr)
{
	setupIOConfiguration(true, false);

	mode = moduleParams.addEnumParameter("Mode", "The way to set the frequency");
	mode->addOption("Frequency", FREQUENCY)->addOption("Time", TIME)->addOption("BPM", BPM);

	onTime = moduleParams.addFloatParameter("ON Time", "Relative amount of time the metronome stays valid (depending on the frequency) when triggered", .5f, 0, 1);
	random = moduleParams.addFloatParameter("Randomness", "Amount of randomness in each call", 0, 0, 1);

	tapTempoIntervalsMax = moduleParams.addIntParameter("Tap tempo averaging", "How many intervals do you want to use in averaging ? 0 means all",4,1);
	tapTempo = moduleParams.addTrigger("Tap Tempo", "press me at least twice to set tempo");
	resetTime = moduleParams.addTrigger("Rest Time", "Reset the metronome time");

	tick = valuesCC.addBoolParameter("Tick", "When the metronome is ticking", false);

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	updateFreqParam();

	startThread();
}

MetronomeModule::~MetronomeModule()
{
	stopThread(1000);
}

void MetronomeModule::updateFreqParam()
{
	stopThread(1000);

	if (freqTimeBpm != nullptr)
	{
		moduleParams.removeControllable(freqTimeBpm);
		freqTimeBpm = nullptr;
	}

	MetroMode m = mode->getValueDataAsEnum<MetroMode>();
	switch (m)
	{
	case FREQUENCY:
		freqTimeBpm = moduleParams.addFloatParameter("Frequency", "Frequency of the timer, in Hz (the greater the value, the faster the tempo)", 1, 0.0001f, 100);
		break;

	case TIME:
		freqTimeBpm = moduleParams.addFloatParameter("Time", "Time between each ticks (the greater the value, the lower the tempo)", 1, .001f);
		freqTimeBpm->defaultUI = FloatParameter::TIME;
		break;

	case BPM:
		freqTimeBpm = moduleParams.addFloatParameter("BPM", "Beats per minutes (the greater the value, the faster the tempo)", 60, 1);
		break;

	}
	moduleParams.controllables.move(moduleParams.controllables.indexOf(freqTimeBpm), moduleParams.controllables.indexOf(mode)+1);
	queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerNeedsRebuild, this));

	startThread();
}

void MetronomeModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);

	if (p == enabled)
	{
		if (enabled->boolValue()) startThread();
		else stopThread(1000);
	}
}

void MetronomeModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == freqTimeBpm || c == random || c == mode)
	{
		if (c == mode) updateFreqParam();
		notify(); //forces the thread to update
	} 
	else if (c == tapTempo)
	{
		tapTempoPressed();
	}
	else if (c == resetTime)
	{
		stopThread(20);
		startThread();
	}
}

void MetronomeModule::run()
{
	if (!enabled->boolValue()) return;

	Random r;
	while (!threadShouldExit())
	{	

		float freq = 1;
		
		MetroMode m = mode->getValueDataAsEnum<MetroMode>();
		switch (m)
		{
		case FREQUENCY:
			freq = freqTimeBpm->floatValue();
			break;

		case TIME:
			freq = 1.0f / freqTimeBpm->floatValue();
			break;

		case BPM:
			freq = freqTimeBpm->floatValue() / 60.f;
			break;
		}

		if(random->floatValue() > 0) freq += (r.nextFloat() * 2 - 1) * random->floatValue();

		//on phase
		int msBeforeON  = Time::getMillisecondCounter();
		tick->setValue(true);
		inActivityTrigger->trigger();
		int msONDiff = Time::getMillisecondCounter() - msBeforeON;

		int msToOff = (1000 / freq) * onTime->floatValue();

		int realMSToOff = msToOff - msONDiff;

		if (realMSToOff > 0)
		{
			bool hasBeenSignalled = wait(realMSToOff);
			if (hasBeenSignalled) continue;
		}

		//off phase
		int msBeforeOFF = Time::getMillisecondCounter();
		tick->setValue(false);
		int msOFFDiff = Time::getMillisecondCounter() - msBeforeOFF;

		int msToOn = (1000 / freq) * (1 - onTime->floatValue());
		int realMSToOn = msToOn - msOFFDiff;

		if (realMSToOn > 0) wait(realMSToOn);
	}
	
}

void MetronomeModule::tapTempoPressed()
{
	double now = Time::getMillisecondCounterHiRes();
	double delta = now - TSTapTempoLastPressed;
	tapTempoHistory.add(now);
	if (delta < 5000 && delta > 0) { // arbitrary value to avoid very low BPM value on first press
		delta = 0;
		int intervals = tapTempoIntervalsMax->getValue();
		if (intervals > 0) {
			while (tapTempoHistory.size() > intervals + 1) {
				tapTempoHistory.remove(0);
			}
		}
		for (int i = 1; i < tapTempoHistory.size(); i++) {
			delta += tapTempoHistory[i] - tapTempoHistory[i-1];
		}
		delta = delta / (double)(tapTempoHistory.size()-1);

		MetroMode m = mode->getValueDataAsEnum<MetroMode>();
		switch (m)
		{
		case FREQUENCY:
			freqTimeBpm->setValue(1000./delta);
			break;

		case TIME:
			freqTimeBpm->setValue(delta/1000.);
			break;

		case BPM:
			freqTimeBpm->setValue(60000. / delta);
			break;
		}
	}
	else // first hit in a long serie
	{
		tapTempoHistory.clear();
		tapTempoHistory.add(now);
	}
	TSTapTempoLastPressed = now;
}