/*
  ==============================================================================

    StateTransition.h
    Created: 28 Oct 2016 8:20:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Processor/Action/Action.h"

class State;

class StateTransition :
	public Action
{
public:
	StateTransition(State * source = nullptr, State * dest = nullptr);
	~StateTransition();

	WeakReference<State> sourceState;
	WeakReference<State> destState;

	virtual void clearItem() override;

	var getJSONData() override;
	void loadJSONDataInternal(var data) override;

	void triggerConsequences(bool triggerTrue, int iterationIndex = 0);
};