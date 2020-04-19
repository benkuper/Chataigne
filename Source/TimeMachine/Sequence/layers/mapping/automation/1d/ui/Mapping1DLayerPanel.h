/*
  ==============================================================================

    Mapping1DLayerPanel.h
    Created: 30 Mar 2020 12:44:39pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../ui/AutomationMappingLayerPanel.h"
#include "../Mapping1DLayer.h"

class Mapping1DLayerPanel :
    public AutomationMappingLayerPanel
{
public:
    Mapping1DLayerPanel(Mapping1DLayer* layer);
    virtual ~Mapping1DLayerPanel();

    Mapping1DLayer* ml;
    std::unique_ptr<ControllableEditor> valueRangeUI;
    std::unique_ptr<ControllableEditor> rangeRemapUI;

    void resizedInternalPanelContent(Rectangle<int>& r) override;
};