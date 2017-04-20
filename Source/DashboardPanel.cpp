/*
  ==============================================================================

    DashboardPanel.cpp
    Created: 19 Apr 2017 10:58:11pm
    Author:  Ben

  ==============================================================================
*/

#include "DashboardPanel.h"

DashboardPanel::DashboardPanel(const String &contentName) :
	ShapeShifterContentComponent(contentName),
	managerUI(DashboardManager::getInstance()),
	currentDashboard(nullptr)
{
	contentIsFlexible = true; 
	
	addAndMakeVisible(&managerUI);
	InspectableSelectionManager::getInstance()->addSelectionListener(this);
	DashboardManager::getInstance()->addBaseManagerListener(this);
	
}

DashboardPanel::~DashboardPanel()
{
	if(DashboardManager::getInstanceWithoutCreating() != nullptr) DashboardManager::getInstance()->removeBaseManagerListener(this);
	if (InspectableSelectionManager::getInstanceWithoutCreating() != nullptr) InspectableSelectionManager::getInstance()->removeSelectionListener(this);
}

void DashboardPanel::setCurrentDashboard(Dashboard * d)
{
	if (currentDashboard == d) return;

	if (view != nullptr)
	{
		removeChildComponent(view);
		view = nullptr;
	}

	currentDashboard = d;

	if (currentDashboard != nullptr)
	{
		view = new DashboardView(&currentDashboard->itemManager);
		addAndMakeVisible(view);
	}

	resized();
}

void DashboardPanel::resized()
{
	Rectangle<int> r = getLocalBounds();
	managerUI.setBounds(r.removeFromLeft(100));
	if (view != nullptr)
	{
		view->setBounds(r);
	}
}

void DashboardPanel::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::getInstance()->isEmpty()) return;
	Dashboard * cc = dynamic_cast<Dashboard *>(InspectableSelectionManager::getInstance()->currentInspectables[0]);
	if (cc == nullptr) return;
	setCurrentDashboard(cc);
}

void DashboardPanel::itemRemoved(Dashboard * d)
{
	if (currentDashboard != nullptr && currentDashboard == d) setCurrentDashboard(nullptr);
}
