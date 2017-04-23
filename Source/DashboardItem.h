/*
  ==============================================================================

    DashboardItem.h
    Created: 19 Apr 2017 11:06:51pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDITEM_H_INCLUDED
#define DASHBOARDITEM_H_INCLUDED

#include "BaseItem.h"
#include "TargetParameter.h"

class DashboardItemUI;

class DashboardItem :
	public BaseItem
{
public:
	DashboardItem(bool canHaveScripts = false);
	~DashboardItem();

	TargetParameter * target;

	void onContainerParameterChangedInternal(Parameter * p) override;
};




#endif  // DASHBOARDITEM_H_INCLUDED
