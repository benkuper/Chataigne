/*
  ==============================================================================

    AutomationMappingLayerPanel.h
    Created: 25 Mar 2020 1:46:10pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ui/MappingLayerPanel.h"
#include "../AutomationMappingLayer.h"

class AutomationMappingLayerPanel :
	public MappingLayerPanel
{
public:
	AutomationMappingLayerPanel(AutomationMappingLayer* layer);
	virtual ~AutomationMappingLayerPanel();

    std::unique_ptr<BoolToggleUI> armUI;

	AutomationMappingLayer* aml;
    

    virtual void resizedInternalPanelContent(Rectangle<int>& r) override;
};