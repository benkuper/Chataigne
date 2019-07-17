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
		colorManagerUI = nullptr;
	}

	for(auto &aui :automationsUI) removeChildComponent(aui);
	automationsUI.clear();

	int index = 0;
	for (auto &a : mappingLayer->automations)
	{
		Colour c = Colours::white;
		if (index == 0 && mappingLayer->automations.size() > 1) c = RED_COLOR;
		if (index == 1) c = GREEN_COLOR;
		if (index == 2) c = BLUE_COLOR;
		AutomationUI * aui = new AutomationUI(a,c);
		aui->autoSwitchMode = false;
		aui->setViewMode(AutomationUI::VIEW);

		addAndMakeVisible(aui);
		automationsUI.add(aui);
		index++;
	}


	if (mappingLayer->mode->getValueDataAsEnum<MappingLayer::Mode>() == MappingLayer::MODE_COLOR)
	{
		colorManagerUI.reset(new TimeColorManagerUI(mappingLayer->timeColorManager.get()));
		addAndMakeVisible(colorManagerUI.get());
		colorManagerUI->toBack();
	}

	needle.toFront(false);
	resized();
	
}

void MappingLayerTimeline::updateContent()
{
	for (auto &aui : automationsUI) aui->setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
	if (colorManagerUI != nullptr) colorManagerUI->setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
}

void MappingLayerTimeline::resized()
{
	for (auto &aui : automationsUI) aui->setBounds(getLocalBounds());
	if (colorManagerUI != nullptr) colorManagerUI->setBounds(getLocalBounds());
}


void MappingLayerTimeline::controllableFeedbackUpdateInternal(Controllable * c)
{
	SequenceLayerTimeline::controllableFeedbackUpdateInternal(c);

	if (c == mappingLayer->mode)
	{
		setupUIForLayerMode();
	} else if (c == mappingLayer->recorder.arm)
	{
		needle.timeBarColor = mappingLayer->recorder.arm->boolValue() ? Colours::red : needle.defaultTimeBarColor;
		repaint();
	}
}

void MappingLayerTimeline::inspectableSelectionChanged(Inspectable * i)
{
	if (i == mappingLayer)
	{
		for (auto &aui : automationsUI) aui->setViewMode(mappingLayer->isSelected?AutomationUI::EDIT:AutomationUI::VIEW);
	}
}
