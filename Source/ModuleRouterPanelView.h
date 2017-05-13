/*
  ==============================================================================

    ModuleRouterPanelView.h
    Created: 29 Apr 2017 10:36:56am
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEROUTERPANELVIEW_H_INCLUDED
#define MODULEROUTERPANELVIEW_H_INCLUDED

#include "ModuleRouterManager.h"
#include "ModuleRouterView.h"

class ModuleRouterPanelView :
	public ShapeShifterContentComponent
{
public:
	ModuleRouterPanelView(const String &contentName, ModuleRouterManager * manager);
	~ModuleRouterPanelView();

	BaseManagerUI<ModuleRouterManager, ModuleRouter, BaseItemUI<ModuleRouter>> mui;
	ModuleRouterView view;

	static ModuleRouterPanelView * create(const String & contentName) { return new ModuleRouterPanelView(contentName, ModuleRouterManager::getInstance()); }

	void resized() override;
};



#endif  // MODULEROUTERPANELVIEW_H_INCLUDED
