/*
  ==============================================================================

    DashboardPanel.h
    Created: 19 Apr 2017 10:58:11pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDPANEL_H_INCLUDED
#define DASHBOARDPANEL_H_INCLUDED

#include "DashboardManagerUI.h"
#include "DashboardView.h"
#include "ShapeShifterContent.h"
#include "InspectableSelectionManager.h"

class DashboardPanel :
	public ShapeShifterContentComponent,
	public InspectableSelectionManager::Listener,
	public DashboardManager::Listener
{
public:
	DashboardPanel(const String &contentName);
	~DashboardPanel();

	DashboardManagerUI managerUI;
	ScopedPointer<DashboardView> view;

	Dashboard * currentDashboard;
	void setCurrentDashboard(Dashboard *);

	void resized();

	void inspectablesSelectionChanged();

	void itemRemoved(Dashboard *) override;
};


#endif  // DASHBOARDPANEL_H_INCLUDED
