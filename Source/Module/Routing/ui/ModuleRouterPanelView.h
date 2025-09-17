/*
  ==============================================================================

    ModuleRouterPanelView.h
    Created: 29 Apr 2017 10:36:56am
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleRouterPanelView :
	public ShapeShifterContentComponent
{
public:
	ModuleRouterPanelView(const String &contentName, ModuleRouterManager * manager);
	~ModuleRouterPanelView();

	ManagerUI<ModuleRouterManager, ModuleRouter, BaseItemUI<ModuleRouter>> mui;
	
	ModuleRouterView view;

	static ModuleRouterPanelView * create(const String & contentName) { return new ModuleRouterPanelView(contentName, ModuleRouterManager::getInstance()); }

	void resized() override;
};