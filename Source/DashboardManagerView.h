/*
  ==============================================================================

    DashboardManagerView.h
    Created: 19 Apr 2017 10:58:11pm
    Author:  Ben

  ==============================================================================
*/

#ifndef DASHBOARDMANAGERVIEW_H_INCLUDED
#define DASHBOARDMANAGERVIEW_H_INCLUDED

#include "DashboardManagerUI.h"
#include "DashboardView.h"
#include "ShapeShifterContent.h"
#include "InspectableSelectionManager.h"

class DashboardManagerView :
	public ShapeShifterContentComponent,
	public InspectableSelectionManager::Listener,
	public DashboardManager::Listener
{
public:
	DashboardManagerView(const String &contentName);
	~DashboardManagerView();

	DashboardManagerUI managerUI;
	ScopedPointer<DashboardView> view;

	Dashboard * currentDashboard;
	void setCurrentDashboard(Dashboard *);

    void resized() override;

	void inspectablesSelectionChanged() override;

	void itemRemoved(Dashboard *) override;
};


#endif  // DASHBOARDMANAGERVIEW_H_INCLUDED
