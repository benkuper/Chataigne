/*
  ==============================================================================

    DashboardItemManager.h
    Created: 19 Apr 2017 11:06:56pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDITEMMANAGER_H_INCLUDED
#define DASHBOARDITEMMANAGER_H_INCLUDED

#include "BaseManager.h"
#include "DashboardItem.h"

class DashboardItemManager :
	public BaseManager<DashboardItem>
{
public:
	DashboardItemManager();
	~DashboardItemManager();

	DashboardItem * addItem(const Point<float> &initialPosition);

};



#endif  // DASHBOARDITEMMANAGER_H_INCLUDED
