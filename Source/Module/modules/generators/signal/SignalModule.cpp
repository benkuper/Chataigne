/*
  ==============================================================================

	SignalModule.cpp
	Created: 30 Dec 2017 10:09:43pm
	Author:  Ben

  ==============================================================================
*/
#include "Module/ModuleIncludes.h"

SignalModule::SignalModule() :
	Module(getTypeString()),
	Thread("Signal"),
	progression(0),
	value(nullptr),
	curRandom(0),
	prevRandomProg(0)
{
	setupIOConfiguration(true, false);

	type = moduleParams.addEnumParameter("Type", "Signal type");
	type->addOption("Sine", SINE)->addOption("Saw", SAW)->addOption("Reverse Saw", SAW_REVERSE)->addOption("Triangle", TRIANGLE)->addOption("Perlin", PERLIN)->addOption("Random", RANDOM)->addOption("Custom", CUSTOM);
	refreshRate = moduleParams.addFloatParameter("Refresh Rate", "Time interval between value updates, in Hz", 30, 1, 200);
	range = moduleParams.addPoint2DParameter("Range", "Min and max values of the signal");
	range->setPoint(0, 1);

	frequency = moduleParams.addFloatParameter("Frequency", "Frequency of the signal", 1);
	phaseOffset = moduleParams.addFloatParameter("Phase Offset", "Global time offset", 0);

	octaves = moduleParams.addIntParameter("Octaves", "Octave parameter for perlin noise", 3, 1, 100, false);

	offsetsNumber = moduleParams.addIntParameter("Offset values", "Number of values spreaded", 0, 0);
	offsetCycles = moduleParams.addFloatParameter("Spread cycles", "number of cycles for spreaded values", 1, 0);
	resetTrigger = moduleParams.addTrigger("Reset", "This will reset the time");

	customCurve = nullptr;

	tapTempoIntervalsMax = moduleParams.addIntParameter("Tap tempo averaging", "How many intervals do you want to use in averaging ? 0 means all", 4, 1);
	tapTempoPerCycle = moduleParams.addIntParameter("Tap tempo per cycle", "Type X times to make a cycle", 4, 1);
	tapTempo = moduleParams.addTrigger("Tap Tempo", "press me at least twice to set tempo");

	value = valuesCC.addFloatParameter("Value", "The signal value", 0, 0, 1);

	for (auto& c : valuesCC.controllables) c->isControllableFeedbackOnly = true;

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

void SignalModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == range)
	{
		if (value != nullptr)
		{
			value->setRange(jmin(range->x, range->y), jmax(range->x, range->y));
			for (int i = 0; i < offsetValues.size(); i++) offsetValues[i]->setRange(value->minimumValue, value->maximumValue);
		}
	}
	else if (c == type)
	{
		octaves->setEnabled(type->getValueDataAsEnum<SignalType>() == PERLIN);

		if (type->getValueDataAsEnum<SignalType>() == CUSTOM)
		{
			customCurve = new Automation("Custom Curve");
			customCurve->addKey(0, 0, false);
			customCurve->items[0]->easingType->setValueWithData(Easing::BEZIER);
			customCurve->addKey(1, 1, false);
			moduleParams.addChildControllableContainer(customCurve, true);
		}
		else if (customCurve != nullptr)
		{
			moduleParams.removeChildControllableContainer(customCurve);
			customCurve = nullptr;
		}



	}
	else if (c == offsetsNumber)
	{
		createOffsetValues();
	}
	else if (c == resetTrigger)
	{
		progression = 0;
	}
	else if (c == tapTempo)
	{
		tapTempoPressed();
	}
}

void SignalModule::run()
{
	if (!enabled->boolValue()) return;

	int lastUpdateMS = Time::getMillisecondCounter();

	while (!threadShouldExit())
	{
		int curTime = Time::getMillisecondCounter();

		if (frequency->floatValue() > 0)
		{
			SignalType t = type->getValueDataAsEnum<SignalType>();

			float progWithPhaseOffset = progression + phaseOffset->floatValue();

			float val = getValueFromProgression(t, progWithPhaseOffset);

			int nOffsets = offsetsNumber->getValue();
			if (nOffsets > 0) {
				float delta = offsetCycles->getValue();
				delta /= nOffsets + 1;

				for (int i = 0; i < nOffsets; i++) {
					float offset = delta * (i + 1);
					offset = progWithPhaseOffset - offset;
					float v = getValueFromProgression(t, offset, i + 1);
					if (offsetValues.size() > i) {
						offsetValues[i]->setNormalizedValue(v);
					}
				}
			}

			int msDiff = curTime - lastUpdateMS;
			progression += msDiff * frequency->floatValue() / 1000.0f;


			value->setNormalizedValue(val);
			inActivityTrigger->trigger();
		}

		lastUpdateMS = curTime;

		int waitMS = 1000.0f / refreshRate->floatValue();
		int afterProcessDiff = Time::getMillisecondCounter() - curTime;
		int realMSToWait = jmax(0, waitMS - afterProcessDiff);
		wait(realMSToWait);
	}
}

void SignalModule::createOffsetValues()
{
	int actual = offsetValues.size();
	int asked = offsetsNumber->getValue();

	if (asked > actual) {
		for (int i = actual; i < asked; i++) {
			FloatParameter* temp = valuesCC.addFloatParameter("Offset " + String(i + 1), "Value for offset " + String(i + 1), 0, 0, 1);
			temp->isControllableFeedbackOnly = true;
			temp->setRange(value->minimumValue, value->maximumValue);
			offsetValues.add(temp);
		}
	}
	else if (asked < actual) {
		for (int i = actual - 1; i >= asked; i--) {
			valuesCC.removeControllable(offsetValues[i]);
			offsetValues.removeLast();
		}
	}

	curRandom.resize(asked);
	prevRandomProg.resize(asked);

}

float SignalModule::getValueFromProgression(SignalType t, float prog, int offsetIndex) {
	float val = 0;
	switch (t)
	{
	case SINE:
		val = sinf(prog * MathConstants<float>::pi * 2) * .5f + .5f;
		break;

	case TRIANGLE:
		val = fabsf(fmodf(prog, 2) - 1);
		break;

	case SAW:
		val = fmodf(prog, 1);
		break;

	case SAW_REVERSE:
		val = 1 - fmodf(prog, 1);
		break;

	case RANDOM:
	{
		int floorProg = floorf(prog);
		if (floorProg != prevRandomProg[offsetIndex])
		{
			curRandom.set(offsetIndex, random.nextFloat());
			prevRandomProg.set(offsetIndex, floorProg);
		}
		val = curRandom[offsetIndex];
	}
	break;

	case PERLIN:
		val = perlin.octaveNoise0_1(prog, octaves->intValue());
		break;

	case CUSTOM:
		if (customCurve != nullptr) {
			val = customCurve->getValueAtPosition(fmodf(prog, 1));
		}
		break;
	}
	return val;
}



void SignalModule::tapTempoPressed()
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
			delta += tapTempoHistory[i] - tapTempoHistory[i - 1];
		}
		delta = delta / (double)(tapTempoHistory.size() - 1);
		delta = delta * (float)tapTempoPerCycle->getValue();

		frequency->setValue(1000. / delta);
	}
	else // first hit in a long serie
	{
		tapTempoHistory.clear();
		tapTempoHistory.add(now);
	}
	TSTapTempoLastPressed = now;
}