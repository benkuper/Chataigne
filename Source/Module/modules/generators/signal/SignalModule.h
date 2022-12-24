/*
  ==============================================================================

    SignalModule.h
    Created: 30 Dec 2017 10:09:43pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

using namespace siv;

class SignalModule :
	public Module,
	public Thread
{
public:
	SignalModule();
	~SignalModule();

	enum SignalType { SINE, SAW, SAW_REVERSE, TRIANGLE, PERLIN, RANDOM, CUSTOM };

	float progression;

	EnumParameter * type;
	FloatParameter * refreshRate;

	Trigger* resetTrigger;
	FloatParameter * frequency;
	FloatParameter* phaseOffset;
	Point2DParameter * range;

	IntParameter * offsetsNumber;
	FloatParameter * offsetCycles;
	Array<FloatParameter *> offsetValues;

	FloatParameter * value;
	
	//Perlin
	IntParameter * octaves;
	PerlinNoise perlin;

	//random
	Array<float> curRandom;
	Array<int> prevRandomProg;
	Random random;

	// custom 
	Automation * customCurve;

	// tap tempo
	Trigger* tapTempo;
	double TSTapTempoLastPressed = 0;
	Array<double> tapTempoHistory;
	IntParameter* tapTempoIntervalsMax;
	IntParameter* tapTempoPerCycle;

	void tapTempoPressed();

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	String getTypeString() const override { return "Signal"; }
	static SignalModule * create() { return new SignalModule(); }

	// offsets
	void createOffsetValues();

	// Inherited via Timer
	virtual void run() override;

	float getValueFromProgression(SignalType t, float prog, int offsetIndex = 0);
};