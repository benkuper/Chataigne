/*
  ==============================================================================

    ActionManagerUI.cpp
    Created: 28 Oct 2016 8:05:53pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionManagerUI.h"


ActionManagerUI::ActionManagerUI(ActionManager * _manager) :
	BaseManagerUI<ActionManager, Action, ActionUI>("Actions", _manager)
{
	addItemText = "Add Action";

	addExistingItems();
}

ActionManagerUI::~ActionManagerUI()
{

}

void ActionManagerUI::addItemFromMenu(Point<int>)
{
	Action * a = manager->addItem();
	a->cdm.addItem();
	a->csm.addItem();
}
