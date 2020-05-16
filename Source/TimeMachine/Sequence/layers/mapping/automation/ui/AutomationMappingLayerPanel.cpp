/*
  ==============================================================================

    AutomationMappingLayerPanel.cpp
    Created: 25 Mar 2020 1:46:10pm
    Author:  bkupe

  ==============================================================================
*/

#include "AutomationMappingLayerPanel.h"
#include "UI/ChataigneAssetManager.h"

AutomationMappingLayerPanel::AutomationMappingLayerPanel(AutomationMappingLayer* layer) :
    MappingLayerPanel(layer),
    aml(layer)
{
    armUI.reset(aml->recorder.arm->createToggle());
    addAndMakeVisible(armUI.get());
}

AutomationMappingLayerPanel::~AutomationMappingLayerPanel()
{
}

void AutomationMappingLayerPanel::resizedInternalPanelContent(Rectangle<int>& r)
{
    MappingLayerPanel::resizedInternalPanelContent(r);
    r.removeFromTop(2);
    armUI->setBounds(r.removeFromTop(16).reduced(4, 0));
}
