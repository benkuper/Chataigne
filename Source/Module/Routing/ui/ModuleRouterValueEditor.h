/*
  ==============================================================================

    ModuleRouterValueEditor.h
    Created: 30 Apr 2017 2:41:54pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleRouterValueEditor : 
	public ItemUI<ModuleRouterValue>,
	public ModuleRouterValue::ValueListener
{
public:
	ModuleRouterValueEditor(ModuleRouterValue * mrv);
	~ModuleRouterValueEditor();

	Label valueLabel;
	std::unique_ptr<ControllableUI> valueUI;
	OwnedArray<ControllableEditor> routeParamsUI;

	void buildRouteParamsUI();

	void resizedHeader(Rectangle<int> &r) override;

	void routeParamsChanged(ModuleRouterValue *) override;
};