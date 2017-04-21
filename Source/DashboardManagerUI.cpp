/*
  ==============================================================================

	DashboardManagerUI.cpp
	Created: 19 Apr 2017 11:00:50pm
	Author:  Ben

  ==============================================================================
*/

#include "DashboardManagerUI.h"

DashboardManagerUI::DashboardManagerUI(DashboardManager * manager) :
	BaseManagerUI("Dashboards", manager)
{
	bgColor = bgColor.brighter();
}

DashboardManagerUI::~DashboardManagerUI()
{

}
