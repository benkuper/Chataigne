/*
  ==============================================================================

    ModuleRouterValueEditor.h
    Created: 30 Apr 2017 2:41:54pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#ifndef MODULEROUTERVALUEDITOR_H_INCLUDED
#define MODULEROUTERVALUEDITOR_H_INCLUDED


#include "../ModuleRouterValue.h"

class ModuleRouterValueEditor : 
	public BaseItemUI<ModuleRouterValue>,
	public ModuleRouterValue::ValueListener
{
public:
	ModuleRouterValueEditor(ModuleRouterValue * mrv);
	~ModuleRouterValueEditor();

	Label valueLabel;
	ScopedPointer<ControllableUI> valueUI;
	OwnedArray<ControllableEditor> routeParamsUI;

	void buildRouteParamsUI();

	void resizedInternalHeader(Rectangle<int> &r) override;

	void routeParamsChanged(ModuleRouterValue *) override;
};

#endif