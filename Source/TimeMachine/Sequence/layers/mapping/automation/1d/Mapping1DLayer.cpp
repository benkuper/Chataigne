/*
  ==============================================================================

    Mapping1DLayer.cpp
    Created: 25 Mar 2020 1:20:57pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping1DLayer.h"
#include "ui/Mapping1DLayerPanel.h"

Mapping1DLayer::Mapping1DLayer(Sequence* s, var params) :
    AutomationMappingLayer(getTypeString(), s, params)
{
    itemDataType = "MappingLayer";
    helpID = "MappingLayer";

    addChildControllableContainer(&automation1D);

    setupAutomation(&automation1D);
    setupMappingInputParameter(automation1D.value);
}

Mapping1DLayer::~Mapping1DLayer()
{
}

var Mapping1DLayer::getValueAtPosition(float position)
{
    return automation1D.getValueAtPosition(position);
}

SequenceLayerPanel* Mapping1DLayer::getPanel()
{
    return new Mapping1DLayerPanel(this);
}
