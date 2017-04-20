/*
  ==============================================================================

    DashboardTargetItem.cpp
    Created: 20 Apr 2017 2:21:33pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardTargetItem.h"
#include "DashboardTargetItemUI.h"

DashboardTargetItem::DashboardTargetItem() :
	DashboardItem(false)
{
	target = addTargetParameter("Target", "Target Item to show", nullptr);
}

DashboardTargetItem::~DashboardTargetItem()
{
}

DashboardItemUI * DashboardTargetItem::getItemUI()
{
	return new DashboardTargetItemUI(this);
}
