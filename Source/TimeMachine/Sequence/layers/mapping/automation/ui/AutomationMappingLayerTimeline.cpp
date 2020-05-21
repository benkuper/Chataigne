/*
  ==============================================================================

    AutomationMappingLayerTimeline.cpp
    Created: 25 Mar 2020 1:45:56pm
    Author:  bkupe

  ==============================================================================
*/

#include "AutomationMappingLayerTimeline.h"

AutomationMappingLayerTimeline::AutomationMappingLayerTimeline(AutomationMappingLayer* layer) :
    MappingLayerTimeline(layer, new AutomationUI(layer->automation)),
    aml(layer)
{
    automationUI = ((AutomationUI*)mainComponent.get());
    needle.timeBarColor = aml->recorder.arm->boolValue() ? Colours::red : needle.defaultTimeBarColor;

    autoSelectWithChildRespect = false;
}

AutomationMappingLayerTimeline::~AutomationMappingLayerTimeline()
{
}

void AutomationMappingLayerTimeline::updateContent()
{
    MappingLayerTimeline::updateContent();
   automationUI->setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
}

void AutomationMappingLayerTimeline::setSeekManipulationMode(bool isManipulating)
{
    MappingLayerTimeline::setSeekManipulationMode(isManipulating);
    automationUI->setPreviewMode(isManipulating);
}

void AutomationMappingLayerTimeline::controllableFeedbackUpdateInternal(Controllable* c)
{
    MappingLayerTimeline::controllableFeedbackUpdateInternal(c);

    if (c == aml->recorder.arm)
    {
        needle.timeBarColor = aml->recorder.arm->boolValue() ? RED_COLOR : needle.defaultTimeBarColor;
        repaint();
    }
}

void AutomationMappingLayerTimeline::inspectableSelectionChanged(Inspectable* i)
{
   // automationUI->setViewMode(mappingLayer->isSelected ? AutomationUI::EDIT : AutomationUI::VIEW);
}
