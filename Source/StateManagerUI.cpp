/*
  ==============================================================================

    StateManagerUI.cpp
    Created: 28 Oct 2016 8:19:25pm
    Author:  bkupe

  ==============================================================================
*/

#include "StateManagerUI.h"

StateManagerUI::StateManagerUI(const String &contentName, StateManager * _manager) :
	BaseManagerShapeShifterUI<StateManager, State, StateUI>(contentName, _manager)
{
	addItemText = "Add State";
	noItemText = "Add interaction by creating a State";

	addExistingItems();
}

StateManagerUI::~StateManagerUI()
{
}
