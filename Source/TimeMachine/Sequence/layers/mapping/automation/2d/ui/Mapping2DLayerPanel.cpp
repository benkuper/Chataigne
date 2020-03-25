/*
  ==============================================================================

    Mapping2DLayerPanel.cpp
    Created: 23 Mar 2020 5:18:05pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping2DLayerPanel.h"

Mapping2DLayerPanel::Mapping2DLayerPanel(Mapping2DLayer* layer) :
    AutomationMappingLayerPanel(layer),
    mappingLayer(layer),
    curveUI(&layer->curve)
{
    keySyncUI.reset(mappingLayer->curve.keySyncMode->createToggle());
    addAndMakeVisible(keySyncUI.get());
    addAndMakeVisible(&curveUI);
}

Mapping2DLayerPanel::~Mapping2DLayerPanel()
{
}

void Mapping2DLayerPanel::resizedInternalPanelContent(Rectangle<int>& r)
{
    AutomationMappingLayerPanel::resizedInternalPanelContent(r);
    keySyncUI->setBounds(r.removeFromTop(16));
    r.removeFromTop(2);
    curveUI.setBounds(r);
}
