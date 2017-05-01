/*
  ==============================================================================

    DashboardItemManagerUI.cpp
    Created: 23 Apr 2017 12:33:58pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardItemManagerUI.h"

DashboardItemManagerUI::DashboardItemManagerUI(DashboardItemManager * manager) :
	BaseManagerUI("Dashboard", manager)
{
	//bgColor = Colours::purple;
	addItemText = "Add Item";
	//setWantsKeyboardFocus(true);

	addExistingItems(false);
	
	setSize(100, 100);
	resized();
}

DashboardItemManagerUI::~DashboardItemManagerUI()
{
}