/*
  ==============================================================================

    StateManagerUI.cpp
    Created: 28 Oct 2016 8:19:25pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateManagerUI.h"

StateManagerUI::StateManagerUI(StateManager * _manager) :
	BaseManagerUI<StateManager, State, StateUI>("States", _manager)
{
	addItemText = "Add State";
}

StateManagerUI::~StateManagerUI()
{
}
