/*
  ==============================================================================

	Dashboard.cpp
	Created: 19 Apr 2017 10:57:04pm
	Author:  Ben

  ==============================================================================
*/

#include "Dashboard.h"

Dashboard::Dashboard() :
	BaseItem("Dashboard", false)
{
}

Dashboard::~Dashboard()
{
}

var Dashboard::getJSONData()
{
	var data = BaseItem::getJSONData();
	data.getDynamicObject()->setProperty("panelManager", panelManager.getJSONData());
	return data;
}

void Dashboard::loadJSONDataInternal(var data)
{
	BaseItem::loadJSONDataInternal(data);
	panelManager.loadJSONData(data.getProperty("panelManager", var()));
}
