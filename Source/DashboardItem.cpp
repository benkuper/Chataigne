/*
  ==============================================================================

    DashboardItem.cpp
    Created: 19 Apr 2017 11:06:51pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardItem.h"

DashboardItem::DashboardItem(bool canHaveScripts) :
	BaseItem("Dashboard Item",false,canHaveScripts)
{
	target = addTargetParameter("Target", "Target Item to show", nullptr);
}

DashboardItem::~DashboardItem()
{
}

void DashboardItem::onContainerParameterChangedInternal(Parameter * p)
{
	BaseItem::onContainerParameterChangedInternal(p);
	if (p == target)
	{
		nameParam->setValue(target->target->parentContainer->niceName + " : " + target->target->niceName);
	}
}
