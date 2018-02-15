/*
  ==============================================================================

    StateTransition.h
    Created: 28 Oct 2016 8:20:02pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATETRANSITION_H_INCLUDED
#define STATETRANSITION_H_INCLUDED

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

	var getJSONData() override;

	void onContainerTriggerTriggered(Trigger *) override;

};

#endif  // STATETRANSITION_H_INCLUDED
