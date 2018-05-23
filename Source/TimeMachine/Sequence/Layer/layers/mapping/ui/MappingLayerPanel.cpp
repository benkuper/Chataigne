/*
  ==============================================================================

    MappingLayerPanel.cpp
    Created: 20 Nov 2016 3:07:42pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "MappingLayerPanel.h"
#include "UI/ChataigneAssetManager.h"

MappingLayerPanel::MappingLayerPanel(MappingLayer * layer) :
	SequenceLayerPanel(layer),
	mappingLayer(layer)
{

	bgColor = MAPPING_COLOR.withSaturation(.2f).darker(1);
	//modeUI = mappingLayer->mode->createUI();
	//addAndMakeVisible(modeUI);

	if (mappingLayer->automations.size() > 0)
	{
		snapUI = mappingLayer->automations[0]->enableSnap->createImageToggle(ChataigneAssetManager::getInstance()->getToggleBTImage(ChataigneAssetManager::getInstance()->getSnapImage()));
		addAndMakeVisible(snapUI);

		snapSensitivityUI = mappingLayer->automations[0]->snapSensitivity->createSlider();
		addAndMakeVisible(snapSensitivityUI);

		automationInspector = new Inspector(mappingLayer->automations[0]->selectionManager);
		addAndMakeVisible(automationInspector);

	}
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
	if (snapUI != nullptr)
	{
		//r.setHeight(r.getHeight);
		Rectangle<int> cr = r.reduced(2, 0);
		Rectangle<int> scr = cr.removeFromTop(20);
		snapUI->setBounds(scr.removeFromLeft(scr.getHeight()));
		scr.reduce(0, 2);
		scr.removeFromLeft(10);
		snapSensitivityUI->setBounds(scr);	

		cr.removeFromTop(2);
		automationInspector->setBounds(cr);
		
	}
}