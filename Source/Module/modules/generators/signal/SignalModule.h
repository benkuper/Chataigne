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

	enum SignalType { SINE, SAW, TRIANGLE, PERLIN, CUSTOM };

	float progression;

	EnumParameter * type;
	FloatParameter * refreshRate;

	FloatParameter * frequency;
	FloatParameter * amplitude;
	FloatParameter * offset;

	IntParameter * offsetsNumber;
	FloatParameter * offsetCycles;
	std::vector<FloatParameter *> offsetValues;

	FloatParameter * value;
	
	//Perlin
	IntParameter * octaves;
	PerlinNoise perlin;

	// custom 
	Automation * customCurve;

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	String getTypeString() const override { return "Signal"; }
	static SignalModule * create() { return new SignalModule(); }

	// offsets
	void createOffsetValues();

	// Inherited via Timer
	virtual void run() override;

	float getValueFromProgression(SignalType t, float prog);
};