/*
  ==============================================================================

    DashboardTargetItemUI.h
    Created: 20 Apr 2017 2:24:12pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDTARGETITEMUI_H_INCLUDED
#define DASHBOARDTARGETITEMUI_H_INCLUDED

#include "DashboardItemUI.h"
#include "DashboardTargetItem.h"
#include "ControllableUI.h"

class DashboardTargetItemUI :
	public DashboardItemUI
{
public:
	DashboardTargetItemUI(DashboardTargetItem * item);
	~DashboardTargetItemUI();

	DashboardTargetItem * targetItem;
	ScopedPointer<ControllableUI> targetUI;

	Controllable * currentControllable;
	void setControllableUI(Controllable * c);

	void resizedInternalContent(Rectangle<int> &r) override;

	void inspectableDestroyed(Inspectable * i) override;
	void controllableFeedbackUpdateInternal(Controllable * c) override;
};



#endif  // DASHBOARDTARGETITEMUI_H_INCLUDED
