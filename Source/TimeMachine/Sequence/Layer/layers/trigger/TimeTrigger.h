/*
  ==============================================================================

    TimeTrigger.h
    Created: 20 Nov 2016 3:18:20pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once

#include "Common/Processor/Action/Action.h"

class TimeTrigger :
	public Action
{
public:
	TimeTrigger(float time = 0, float flagYPos = 0, const String &name = "Trigger");
	virtual ~TimeTrigger();

	FloatParameter * time;
	BoolParameter * isTriggered;
	
	BoolParameter * isLocked;

	//ui
	FloatParameter * flagY;

	void onContainerParameterChangedInternal(Parameter * p) override;

	void trigger();
};