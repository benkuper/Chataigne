/*
  ==============================================================================

	DashboardItemManager.cpp
	Created: 19 Apr 2017 11:06:56pm
	Author:  Ben

  ==============================================================================
*/

#include "DashboardItemManager.h"
#include "DashboardTargetItem.h"

DashboardItemManager::DashboardItemManager() :
	BaseManager("Dashboard")
{
}

DashboardItemManager::~DashboardItemManager()
{
}

DashboardItem * DashboardItemManager::createItem()
{
	return new DashboardTargetItem();
}

DashboardItem * DashboardItemManager::addItem(const Point<float>& initialPosition)
{
	DashboardItem * s = BaseManager::addItem();
	s->viewUIPosition->setPoint(initialPosition);
	return s;
}
