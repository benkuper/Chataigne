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
}

AutomationMappingLayerPanel::~AutomationMappingLayerPanel()
{
}
