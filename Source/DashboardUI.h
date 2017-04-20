/*
  ==============================================================================

    DashboardUI.h
    Created: 19 Apr 2017 11:06:43pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDUI_H_INCLUDED
#define DASHBOARDUI_H_INCLUDED

#include "Dashboard.h"
#include "BaseItemUI.h"

class DashboardUI :
	public BaseItemUI<Dashboard>
{
public:
	DashboardUI(Dashboard * item);
	~DashboardUI();
};



#endif  // DASHBOARDUI_H_INCLUDED
