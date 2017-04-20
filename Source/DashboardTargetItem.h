/*
  ==============================================================================

    DashboardTargetItem.h
    Created: 20 Apr 2017 2:21:33pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDTARGETITEM_H_INCLUDED
#define DASHBOARDTARGETITEM_H_INCLUDED

#include "DashboardItem.h"

class DashboardTargetItem :
	public DashboardItem
{
public:
	DashboardTargetItem();
	~DashboardTargetItem();

	TargetParameter * target;

	DashboardItemUI * getItemUI() override;
};


#endif  // DASHBOARDTARGETITEM_H_INCLUDED
