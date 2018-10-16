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
	progression(0)
{
	setupIOConfiguration(true, false);

	type = moduleParams.addEnumParameter("Type", "Signal type");
	type->addOption("Sine", SINE)->addOption("Saw",SAW)->addOption("Triangle",TRIANGLE)->addOption("Perlin", PERLIN);
	refreshRate = moduleParams.addFloatParameter("Refresh Rate", "Time interval between value updates, in Hz", 50, 1, 100);
	amplitude = moduleParams.addFloatParameter("Amplitude", "Amplitude of the signal, act as a multiplier", 1);

	frequency = moduleParams.addFloatParameter("Frequency", "Frequency of the signal", 1, 0.0001f, 5);
	octaves = moduleParams.addIntParameter("Octaves", "Octave parameter for perlin noise", 3, 1, 100, false);

	value = valuesCC.addFloatParameter("Value", "The signal value", 0, 0, 1);

	for (auto &c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

	startTimer(0,1000.0f/refreshRate->floatValue());
}

SignalModule::~SignalModule()
{
}

void SignalModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == amplitude)
	{
		value->setRange(0, amplitude->floatValue());
	}else if (c == refreshRate)
	{
		startTimer(0, 1000.0f / refreshRate->floatValue());
	}else if(c == type)	
	{
		octaves->setEnabled(type->getValueDataAsEnum<SignalType>() == PERLIN);
	}
}

void SignalModule::timerCallback(int timerID)
{
	if (!enabled->boolValue()) return;

	if (timerID == 0)
	{
		SignalType t = type->getValueDataAsEnum<SignalType>();

		float val = 0;
		progression = progression + getTimerInterval(0)*frequency->floatValue() / 1000.0f;


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
	}

	
}
