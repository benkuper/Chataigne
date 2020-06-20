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
    valueRangeUI->showLabel = true;
    addAndMakeVisible(valueRangeUI.get());

    rangeRemapUI.reset((ControllableEditor*)ml->automation1D.rangeRemapMode->getEditor(false));
    addAndMakeVisible(rangeRemapUI.get());

    contentComponents.add(valueRangeUI.get());
    contentComponents.add(rangeRemapUI.get());
}

Mapping1DLayerPanel::~Mapping1DLayerPanel()
{
}

void Mapping1DLayerPanel::resizedInternalPanelContent(Rectangle<int>& r)
{
    AutomationMappingLayerPanel::resizedInternalPanelContent(r);
    r.removeFromTop(8);
    valueRangeUI->setBounds(r.removeFromTop(16).reduced(4, 0));
    r.removeFromTop(8);
    rangeRemapUI->setBounds(r.removeFromTop(16).reduced(4, 0));
}
