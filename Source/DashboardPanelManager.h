/*
  ==============================================================================

    DashboardPanelManager.h
    Created: 23 Apr 2017 12:20:13pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDPANELMANAGER_H_INCLUDED
#define DASHBOARDPANELMANAGER_H_INCLUDED

#include "DashboardPanel.h"
#include "BaseManager.h"

class DashboardPanelManager :
	public BaseManager<DashboardPanel>
{
public:
	DashboardPanelManager();
	~DashboardPanelManager();

	DashboardPanel * addItem(const Point<float> &initialPosition);

};





#endif  // DASHBOARDPANELMANAGER_H_INCLUDED
