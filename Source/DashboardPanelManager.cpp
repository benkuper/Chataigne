/*
  ==============================================================================

    DashboardPanelManager.cpp
    Created: 23 Apr 2017 12:20:13pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardPanelManager.h"

DashboardPanelManager::DashboardPanelManager() :
	BaseManager("Panels")
{
}

DashboardPanelManager::~DashboardPanelManager()
{
}

DashboardPanel * DashboardPanelManager::addItem(const Point<float>& initialPosition)
{
	DashboardPanel * s = BaseManager::addItem();
	s->viewUIPosition->setPoint(initialPosition);
	return s;
}
