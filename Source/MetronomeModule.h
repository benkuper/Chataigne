/*
  ==============================================================================

    MetronomeModule.h
    Created: 30 Dec 2017 10:09:22pm
    Author:  Ben

  ==============================================================================
*/

#pragma once
#include "Module.h"

class MetronomeModule :
	public Module,
	public MultiTimer
{
public:
	MetronomeModule();
	~MetronomeModule();

	BoolParameter * tick;
	FloatParameter * frequency;
	FloatParameter * onTime;

	void onContainerParameterChangedInternal(Parameter *) override;

	String getTypeString() const override { return "Metronome"; }
	static MetronomeModule * create() { return new MetronomeModule(); }


	// Inherited via Timer
	virtual void timerCallback(int timerID) override;
};