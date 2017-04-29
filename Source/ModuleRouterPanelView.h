/*
  ==============================================================================

    ModuleRouterPanelView.h
    Created: 29 Apr 2017 10:36:56am
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEROUTERPANELVIEW_H_INCLUDED
#define MODULEROUTERPANELVIEW_H_INCLUDED

#include "ShapeShifterContent.h"
#include "ModuleRouterManager.h"
#include "ModuleRouterView.h"
#include "BaseManagerUI.h"
#include "BaseItemUI.h"

class ModuleRouterPanelView :
	public ShapeShifterContentComponent
{
public:
	ModuleRouterPanelView(const String &contentName);
	~ModuleRouterPanelView();

	BaseManagerUI<ModuleRouterManager, ModuleRouter, BaseItemUI<ModuleRouter>> mui;
	ModuleRouterView view;

	void resized() override;
};



#endif  // MODULEROUTERPANELVIEW_H_INCLUDED
