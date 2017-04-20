/*
  ==============================================================================

    DashboardItem.cpp
    Created: 19 Apr 2017 11:06:51pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardItem.h"
#include "DashboardItemUI.h"

DashboardItem::DashboardItem(bool canHaveScripts) :
	BaseItem("Dashboard Item",false,canHaveScripts)
{

	viewUIPosition = addPoint2DParameter("Position", "Position in State Machine view");
	viewUIPosition->setBounds(-100, -100, 100, 100);
	//viewUIPosition->hideInEditor = true;
	viewUIPosition->hideInOutliner = true;
	viewUIPosition->isTargettable = false;

	viewUISize = addPoint2DParameter("Size", "Size in State Machine view");
	viewUISize->setBounds(30, 60, 500, 500);
	viewUISize->setPoint(100,60);
	//viewUISize->hideInEditor = true;
	viewUISize->hideInOutliner = true;
	viewUISize->isTargettable = false;
}

DashboardItem::~DashboardItem()
{
}

DashboardItemUI * DashboardItem::getItemUI()
{
	return new DashboardItemUI(this);
}
