/*
  ==============================================================================

    StateManagerUI.cpp
    Created: 28 Oct 2016 8:19:25pm
    Author:  bkupe

  ==============================================================================
*/

StateManagerUI::StateManagerUI(const String &contentName, StateManager * _manager) :
	ManagerShapeShifterUI<StateManager, State, StateUI>(contentName, _manager)
{
	addItemText = "Add State";
	noItemText = "Add interaction by creating a State";

	setShowSearchBar(true);
	addExistingItems();
}

StateManagerUI::~StateManagerUI()
{
}
