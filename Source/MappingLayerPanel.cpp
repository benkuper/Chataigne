/*
  ==============================================================================

    MappingLayerPanel.cpp
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayerPanel.h"
#include "ChataigneAssetManager.h"

MappingLayerPanel::MappingLayerPanel(MappingLayer * layer) :
	SequenceLayerPanel(layer),
	mappingLayer(layer)
{

	bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);
	//modeUI = mappingLayer->mode->createUI();
	//addAndMakeVisible(modeUI);

	snapUI = mappingLayer->automations[0]->enableSnap->createImageToggle(ChataigneAssetManager::getInstance()->getToggleBTImage(ChataigneAssetManager::getInstance()->getSnapImage()));
	addAndMakeVisible(snapUI);

	snapSensitivityUI = mappingLayer->automations[0]->snapSensitivity->createSlider();
	addAndMakeVisible(snapSensitivityUI);
}

MappingLayerPanel::~MappingLayerPanel()
{
}

void MappingLayerPanel::resizedInternalHeader(Rectangle<int>& r)
{
	SequenceLayerPanel::resizedInternalHeader(r);
	
}

void MappingLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	r.setHeight(20);
	Rectangle<int> cr = r.reduced(2, 0);
	snapUI->setBounds(cr.removeFromLeft(cr.getHeight()));
	cr.reduce(0, 2);
	cr.removeFromLeft(10);
	snapSensitivityUI->setBounds(cr);
		//modeUI->setBounds(getLocalBounds().reduced(5).removeFromBottom(20));
}
