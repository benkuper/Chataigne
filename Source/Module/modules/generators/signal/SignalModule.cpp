/*
  ==============================================================================

    SignalModule.cpp
    Created: 30 Dec 2017 10:09:43pm
    Author:  Ben

  ==============================================================================
*/

#include "SignalModule.h"

SignalModule::SignalModule() :
	Module(getTypeString()),
	Thread("Signal"),
	progression(0)
{
	setupIOConfiguration(true, false);

	type = moduleParams.addEnumParameter("Type", "Signal type");
	type->addOption("Sine", SINE)->addOption("Saw",SAW)->addOption("Triangle",TRIANGLE)->addOption("Perlin", PERLIN);
	refreshRate = moduleParams.addFloatParameter("Refresh Rate", "Time interval between value updates, in Hz", 50, 1, 200);
	amplitude = moduleParams.addFloatParameter("Amplitude", "Amplitude of the signal, act as a multiplier", 1, 0);
	offset = moduleParams.addFloatParameter("Offset", "Offset the signal value", 0);

	frequency = moduleParams.addFloatParameter("Frequency", "Frequency of the signal", 1, 0.0001f, 5);
	octaves = moduleParams.addIntParameter("Octaves", "Octave parameter for perlin noise", 3, 1, 100, false);

	value = valuesCC.addFloatParameter("Value", "The signal value", 0, 0, 1);

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	startThread();
}

SignalModule::~SignalModule()
{
	stopThread(1000);
}

void SignalModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == enabled)
	{
		if (enabled->boolValue()) startThread();
		else stopThread(1000);
	}
}

void SignalModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == amplitude || c == offset)
	{
		value->setRange(offset->floatValue(), amplitude->floatValue() + offset->floatValue());
	}else if(c == type)	
	{
		octaves->setEnabled(type->getValueDataAsEnum<SignalType>() == PERLIN);
	}
}

void SignalModule::run()
{

	if (!enabled->boolValue()) return;

	int lastUpdateMS = Time::getMillisecondCounter();

	while(!threadShouldExit())
	{
		int curTime = Time::getMillisecondCounter();

		SignalType t = type->getValueDataAsEnum<SignalType>();

		float val = 0;
		
		int msDiff = curTime - lastUpdateMS;
		progression += msDiff * frequency->floatValue() / 1000.0f;

		lastUpdateMS = curTime;

		switch (t)
		{
		case SINE:
			val = sinf(progression*MathConstants<float>::pi*2)*.5f + .5f;
			break;

		case TRIANGLE:
			val = fabsf(fmodf(progression, 2) - 1);
			break;

		case SAW:
			val = fmodf(progression,1);
			break;

		case PERLIN:
			val = perlin.octaveNoise0_1(progression, octaves->intValue());
			break;
		}

		value->setNormalizedValue(val);
		inActivityTrigger->trigger();

		int waitMS = 1000.0f / refreshRate->floatValue();
		int afterProcessDiff = Time::getMillisecondCounter() - curTime;
		int realMSToWait = jmax(0, waitMS - afterProcessDiff);
		wait(realMSToWait);
	}
}
