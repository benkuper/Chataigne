/*
  ==============================================================================

    MappingLayerTimeline.cpp
    Created: 20 Nov 2016 3:08:35pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayerTimeline.h"

MappingLayerTimeline::MappingLayerTimeline(MappingLayer * layer) :
	SequenceLayerTimeline(layer),
	mappingLayer(layer)
{
	setInterceptsMouseClicks(true, true);
	
	setupUIForLayerMode();
	needle.timeBarColor = mappingLayer->recorder.arm->boolValue() ? Colours::red : needle.defaultTimeBarColor;
	updateContent();
}

MappingLayerTimeline::~MappingLayerTimeline()
{
}

void MappingLayerTimeline::setupUIForLayerMode()
{
	if (colorManagerUI != nullptr)
	{
		removeChildComponent(colorManagerUI.get());
		colorManagerUI.reset();
	}

	if (automationUI != nullptr)
	{
		removeChildComponent(automationUI.get());
		automationUI.reset();
	}


	if (mappingLayer->mode == MappingLayer::MODE_COLOR)
	{
		colorManagerUI.reset(new GradientColorManagerUI(mappingLayer->colorManager.get()));
		addAndMakeVisible(colorManagerUI.get());
		colorManagerUI->toBack();
	}
	else
	{
		automationUI.reset(new AutomationUI(mappingLayer->automation.get()));
		automationUI->autoSwitchMode = false;
		automationUI->setViewMode(AutomationUI::VIEW);
		addAndMakeVisible(automationUI.get());
	}

	needle.toFront(false);
	resized();
	
}

void MappingLayerTimeline::updateContent()
{
	if(automationUI != nullptr) automationUI->setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
	if (colorManagerUI != nullptr) colorManagerUI->setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
}

void MappingLayerTimeline::resized()
{
	if(automationUI != nullptr) automationUI->setBounds(getLocalBounds());
	if (colorManagerUI != nullptr) colorManagerUI->setBounds(getLocalBounds());
}


void MappingLayerTimeline::controllableFeedbackUpdateInternal(Controllable * c)
{
	SequenceLayerTimeline::controllableFeedbackUpdateInternal(c);

	if (c == mappingLayer->recorder.arm)
	{
		needle.timeBarColor = mappingLayer->recorder.arm->boolValue() ? Colours::red : needle.defaultTimeBarColor;
		repaint();
	}
}

void MappingLayerTimeline::inspectableSelectionChanged(Inspectable * i)
{
	if (i == mappingLayer)
	{
		if(automationUI != nullptr) automationUI->setViewMode(mappingLayer->isSelected?AutomationUI::EDIT:AutomationUI::VIEW);
	}
}
