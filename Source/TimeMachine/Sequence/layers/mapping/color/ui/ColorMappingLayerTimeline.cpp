/*
  ==============================================================================

	ColorMappingLayerTimeline.cpp
	Created: 25 Mar 2020 1:45:28pm
	Author:  bkupe

  ==============================================================================
*/

#include "ColorMappingLayerTimeline.h"

ColorMappingLayerTimeline::ColorMappingLayerTimeline(ColorMappingLayer* layer) :
	MappingLayerTimeline(layer, new GradientColorManagerUI(&layer->colorManager)),
	cml(layer)
{
	colorManagerUI = ((GradientColorManagerUI*)mainComponent.get());
}

ColorMappingLayerTimeline::~ColorMappingLayerTimeline()
{
}

void ColorMappingLayerTimeline::updateContent()
{
	colorManagerUI->setViewRange(item->sequence->viewStartTime->floatValue(), item->sequence->viewEndTime->floatValue());
}
