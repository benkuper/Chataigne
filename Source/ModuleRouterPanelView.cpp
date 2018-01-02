/*
  ==============================================================================

	ModuleRouterPanelView.cpp
	Created: 29 Apr 2017 10:36:56am
	Author:  Ben

  ==============================================================================
*/

#include "ModuleRouterPanelView.h"

ModuleRouterPanelView::ModuleRouterPanelView(const String & contentName,ModuleRouterManager * manager) :
	ShapeShifterContentComponent(contentName),
	mui("Routers",manager)
{
    contentIsFlexible = true;
    
	mui.bgColor = BG_COLOR.brighter();
	mui.animateItemOnAdd = true;
	mui.addExistingItems(); //we have to do it here since we don't have a custom child class for managerUI
	mui.addItemText = "Add Router";

	addAndMakeVisible(&mui);
	addAndMakeVisible(&view);

	helpID = "ModuleRouter";
}

ModuleRouterPanelView::~ModuleRouterPanelView()
{
}

void ModuleRouterPanelView::resized()
{
	Rectangle<int> r = getLocalBounds();
	mui.setBounds(r.removeFromLeft(150));
	r.removeFromLeft(2);
	view.setBounds(r);
}
