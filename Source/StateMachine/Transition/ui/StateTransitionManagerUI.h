/*
  ==============================================================================

    StateTransitionManagerUI.h
    Created: 28 Jan 2017 11:01:29am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../../Transition/StateTransitionManager.h"
#include "StateTransitionUI.h"

class StateMachineView;

class StateTransitionManagerUI :
	public BaseManagerUI<StateTransitionManager, StateTransition, StateTransitionUI>
{
public :
	StateTransitionManagerUI(StateMachineView * stateView, StateTransitionManager *);
	~StateTransitionManagerUI();

	StateMachineView * stateView;

	void resized() override;
	
	StateTransitionUI * createUIForItem(StateTransition *) override;
	void addItemUIInternal(StateTransitionUI * sui) override;
};