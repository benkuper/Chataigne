/*
  ==============================================================================

	ModuleRouterPanelView.cpp
	Created: 29 Apr 2017 10:36:56am
	Author:  Ben

  ==============================================================================
*/

#include "ModuleRouterPanelView.h"

ModuleRouterPanelView::ModuleRouterPanelView(const String & contentName) :
	ShapeShifterContentComponent(contentName),
	mui("Routers",ModuleRouterManager::getInstance())
{
    contentIsFlexible = true;
    
	mui.bgColor = BG_COLOR.brighter();
	mui.animateItemOnAdd = true;

	addAndMakeVisible(&mui);
	addAndMakeVisible(&view);
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
