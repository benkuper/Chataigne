/*
  ==============================================================================

	DashboardPanel.cpp
	Created: 23 Apr 2017 12:20:05pm
	Author:  Ben

  ==============================================================================
*/

#include "DashboardPanel.h"

DashboardPanel::DashboardPanel() :
	BaseItem("Panel",true,true)
{
	viewUISize->setPoint(200, 200);
}

DashboardPanel::~DashboardPanel()
{
}

var DashboardPanel::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("itemManager", itemManager.getJSONData());
	return data;
}

void DashboardPanel::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	itemManager.loadJSONData(data.getProperty("itemManager", var()));
}
