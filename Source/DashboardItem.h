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

class DashboardItemUI;

class DashboardItem :
	public BaseItem
{
public:
	DashboardItem(bool canHaveScripts = false);
	~DashboardItem();

	Point2DParameter * viewUIPosition;
	Point2DParameter * viewUISize;

	virtual DashboardItemUI * getItemUI();
};




#endif  // DASHBOARDITEM_H_INCLUDED
