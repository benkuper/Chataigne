/*
  ==============================================================================

    DashboardItemUI.h
    Created: 19 Apr 2017 11:01:13pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDITEMUI_H_INCLUDED
#define DASHBOARDITEMUI_H_INCLUDED

#include "BaseItemUI.h"
#include "DashboardItem.h"

class DashboardItemUI :
	public BaseItemUI<DashboardItem>
{
public:
	DashboardItemUI(DashboardItem * item);
	~DashboardItemUI();

	ScopedPointer<ControllableUI> targetUI;

	Controllable * currentControllable;
	void setControllableUI(Controllable * c);

	void resizedInternalContent(Rectangle<int> &r) override;

	void inspectableDestroyed(Inspectable * i) override;
	void controllableFeedbackUpdateInternal(Controllable * c) override;
};



#endif  // DASHBOARDITEMUI_H_INCLUDED
