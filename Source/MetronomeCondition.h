/*
  ==============================================================================

    MetronomeCondition.h
    Created: 19 Nov 2017 11:14:55pm
    Author:  Ben

  ==============================================================================
*/

#pragma once


#include "Condition.h"

class MetronomeCondition :
	public Condition,
	public MultiTimer
{
public:
	MetronomeCondition();
	~MetronomeCondition();

	FloatParameter * frequency;
	FloatParameter * onTime;

	//Script
	InspectableEditor * getEditor(bool isRoot) override;

	void onContainerParameterChangedInternal(Parameter *) override;
	
	String getTypeString() const override { return "Metronome"; }
	static MetronomeCondition * create() { return new MetronomeCondition(); }


	// Inherited via Timer
	virtual void timerCallback(int timerID) override;

};


