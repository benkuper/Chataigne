/*
  ==============================================================================

    Mapping1DLayerPanel.cpp
    Created: 30 Mar 2020 12:44:39pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping1DLayerPanel.h"

Mapping1DLayerPanel::Mapping1DLayerPanel(Mapping1DLayer* layer) :
    AutomationMappingLayerPanel(layer),
    ml(layer)
{
    valueRangeUI.reset((ControllableEditor *)ml->automation1D.valueRange->getEditor(false));
    addAndMakeVisible(valueRangeUI.get());
}

Mapping1DLayerPanel::~Mapping1DLayerPanel()
{
}

void Mapping1DLayerPanel::resizedInternalPanelContent(Rectangle<int>& r)
{
    AutomationMappingLayerPanel::resizedInternalPanelContent(r);
    valueRangeUI->setBounds(r.removeFromTop(16));
}
