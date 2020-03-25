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
    snapUI.reset(aml->automation->enableSnap->createImageToggle(ChataigneAssetManager::getInstance()->getToggleBTImage(ChataigneAssetManager::getInstance()->getSnapImage())));
    addAndMakeVisible(snapUI.get());

    snapSensitivityUI.reset(aml->automation->snapSensitivity->createSlider());
    addAndMakeVisible(snapSensitivityUI.get());
}

AutomationMappingLayerPanel::~AutomationMappingLayerPanel()
{
}

void AutomationMappingLayerPanel::resizedInternalPanelContent(Rectangle<int>& r)
{
    if (snapUI != nullptr)
    {
        Rectangle<int> scr = r.removeFromTop(20);
        snapUI->setBounds(scr.removeFromLeft(scr.getHeight()));
        scr.reduce(0, 2);
        scr.removeFromLeft(10);
        snapSensitivityUI->setBounds(scr);

        r.removeFromTop(2);
    }
}
