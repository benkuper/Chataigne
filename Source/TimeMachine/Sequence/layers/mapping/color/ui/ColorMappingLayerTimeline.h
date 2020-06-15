/*
  ==============================================================================

    ColorMappingLayerTimeline.h
    Created: 25 Mar 2020 1:45:28pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ColorMappingLayer.h"
#include "../../ui/MappingLayerTimeline.h"

class ColorMappingLayerTimeline :
    public MappingLayerTimeline
{
public:
    ColorMappingLayerTimeline(ColorMappingLayer* layer);
    ~ColorMappingLayerTimeline();

    ColorMappingLayer* cml;
    GradientColorManagerUI* colorManagerUI;

    virtual void updateContent() override;

    virtual void addSelectableComponentsAndInspectables(Array<Component*>& selectables, Array<Inspectable*>& inspectables) override;
};