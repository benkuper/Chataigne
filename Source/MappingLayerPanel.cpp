/*
  ==============================================================================

    MappingLayerPanel.cpp
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayerPanel.h"

MappingLayerPanel::MappingLayerPanel(MappingLayer * layer) :
	SequenceLayerPanel(layer),
	mappingLayer(layer)
{

	bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);
	modeUI = mappingLayer->mode->createUI();
	addAndMakeVisible(modeUI);
}

MappingLayerPanel::~MappingLayerPanel()
{
}

void MappingLayerPanel::resizedInternalHeader(Rectangle<int>& r)
{
	SequenceLayerPanel::resizedInternalHeader(r);
	
	modeUI->setBounds(getLocalBounds().reduced(5).removeFromBottom(20));
}
