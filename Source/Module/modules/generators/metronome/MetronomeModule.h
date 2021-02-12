/*
  ==============================================================================

    MetronomeModule.h
    Created: 30 Dec 2017 10:09:22pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "Module/Module.h"

class MetronomeModule :
	public Module,
	public Thread
{
public:
	MetronomeModule();
	~MetronomeModule();

	BoolParameter * tick;
	FloatParameter * frequency;
	FloatParameter * onTime;
	FloatParameter * random;
	Random rnd;

	void onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c) override;

	virtual void run() override;

	String getTypeString() const override { return "Metronome"; }
	static MetronomeModule * create() { return new MetronomeModule(); }

};