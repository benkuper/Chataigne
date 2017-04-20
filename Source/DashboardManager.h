/*
  ==============================================================================

    DashboardManager.h
    Created: 19 Apr 2017 10:57:53pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDMANAGER_H_INCLUDED
#define DASHBOARDMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "Dashboard.h"

class DashboardManager :
	public BaseManager<Dashboard>
{
public:
	juce_DeclareSingleton(DashboardManager, true);

	DashboardManager();
	~DashboardManager();
};



#endif  // DASHBOARDMANAGER_H_INCLUDED
