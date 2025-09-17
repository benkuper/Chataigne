/*
  ==============================================================================

    StateTransitionManagerUI.h
    Created: 28 Jan 2017 11:01:29am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class StateMachineView;

class StateTransitionManagerUI :
	public ManagerUI<StateTransitionManager, StateTransition, StateTransitionUI>
{
public :
	StateTransitionManagerUI(StateMachineView * stateView, StateTransitionManager *);
	~StateTransitionManagerUI();

	StateMachineView * stateView;

	void resized() override;
	
	StateTransitionUI * createUIForItem(StateTransition *) override;
	void addBaseItemUIInternal(StateTransitionUI * sui) override;
};