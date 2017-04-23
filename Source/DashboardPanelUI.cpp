/*
  ==============================================================================

    DashboardPanelUI.cpp
    Created: 23 Apr 2017 12:20:30pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardPanelUI.h"

DashboardPanelUI::DashboardPanelUI(DashboardPanel * item) :
	BaseItemUI(item,ResizeMode::ALL,true),
	managerUI(&item->itemManager)
{
	addAndMakeVisible(&managerUI);

	resized();
}

DashboardPanelUI::~DashboardPanelUI()
{
}


void DashboardPanelUI::resizedInternalContent(Rectangle<int>& r)
{
	managerUI.setBounds(r);
}