/*
  ==============================================================================

    SignalModule.h
    Created: 30 Dec 2017 10:09:43pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module/Module.h"
#include "PerlinNoise.hpp"

using namespace siv;

class SignalModule :
	public Module,
	public MultiTimer
{
public:
	SignalModule();
	~SignalModule();

	enum SignalType { SINE, SAW, TRIANGLE, PERLIN };

	float progression;

	EnumParameter * type;
	FloatParameter * refreshRate;

	FloatParameter * frequency;
	FloatParameter * amplitude;
	FloatParameter * offset;

	FloatParameter * value;
	

	//Perlin
	IntParameter * octaves;
	PerlinNoise perlin;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	String getTypeString() const override { return "Signal"; }
	static SignalModule * create() { return new SignalModule(); }

	// Inherited via Timer
	virtual void timerCallback(int timerID) override;
};