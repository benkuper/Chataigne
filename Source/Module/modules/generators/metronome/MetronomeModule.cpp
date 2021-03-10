/*
  ==============================================================================

    MetronomeModule.cpp
    Created: 30 Dec 2017 10:09:22pm
    Author:  Ben

  ==============================================================================
*/

MetronomeModule::MetronomeModule() :
	Module(getTypeString()),
	Thread("Metronome")
{
	setupIOConfiguration(true, false);

	frequency = moduleParams.addFloatParameter("Frequency", "Frequency of the timer, in Hz (the greater the value, the faster the tempo)", 1, 0.0001f, 100);
	onTime = moduleParams.addFloatParameter("ON Time", "Relative amount of time the metronome stays valid (depending on the frequency) when triggered", .5f, 0, 1);
	random = moduleParams.addFloatParameter("Randomness", "Amount of randomness in each call", 0, 0, 1);

	tick = valuesCC.addBoolParameter("Tick", "When the metronome is ticking", false);

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	startThread();
}

MetronomeModule::~MetronomeModule()
{
	stopThread(1000);
}

void MetronomeModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == frequency || c == random)
	{
		notify(); //forces the thread to update
	}
}

void MetronomeModule::run()
{
	if (!enabled->boolValue()) return;

	Random r;
	while (!threadShouldExit())
	{
		float freq = frequency->floatValue();
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
