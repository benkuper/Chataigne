/*
  ==============================================================================

    AutomationMappingLayerTimeline.h
    Created: 25 Mar 2020 1:45:56pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "../../ui/MappingLayerTimeline.h"
#include "../AutomationMappingLayer.h"

class AutomationMappingLayerTimeline :
    public MappingLayerTimeline
{
public:
    AutomationMappingLayerTimeline(AutomationMappingLayer* layer);
    ~AutomationMappingLayerTimeline();

    AutomationMappingLayer* aml;
    AutomationUI* automationUI;

    virtual void updateContent() override;

    virtual void controllableFeedbackUpdateInternal(Controllable* c) override;

    virtual void inspectableSelectionChanged(Inspectable* i) override;

};