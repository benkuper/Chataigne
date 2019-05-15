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

	//modeUI = mappingLayer->mode->createUI();
	//addAndMakeVisible(modeUI);

	if (mappingLayer->automations.size() > 0)
	{
		snapUI.reset(mappingLayer->automations[0]->enableSnap->createImageToggle(ChataigneAssetManager::getInstance()->getToggleBTImage(ChataigneAssetManager::getInstance()->getSnapImage())));
		addAndMakeVisible(snapUI.get());

		snapSensitivityUI.reset(mappingLayer->automations[0]->snapSensitivity->createSlider());
		addAndMakeVisible(snapSensitivityUI.get());

		automationInspector.reset(new Inspector(mappingLayer->automations[0]->selectionManager));
		addAndMakeVisible(automationInspector.get());
	}

	mappingLayer->mapping.addAsyncMappingListener(this);

	mappingOutputUI.reset(dynamic_cast<ParameterUI*>(mappingLayer->mapping.outputParam->createDefaultUI()));
	if (mappingOutputUI != nullptr) addAndMakeVisible(mappingOutputUI.get());
	resized();
}

MappingLayerPanel::~MappingLayerPanel()
{
	if(!inspectable.wasObjectDeleted()) mappingLayer->mapping.removeAsyncMappingListener(this);
}

void MappingLayerPanel::resizedInternalHeader(Rectangle<int>& r)
{
	SequenceLayerPanel::resizedInternalHeader(r);
	
}

void MappingLayerPanel::resizedInternalContent(Rectangle<int>& r)
{
	Rectangle<int> cr = r.reduced(2, 0);

	if (snapUI != nullptr)
	{
		Rectangle<int> scr = cr.removeFromTop(20);
		snapUI->setBounds(scr.removeFromLeft(scr.getHeight()));
		scr.reduce(0, 2);
		scr.removeFromLeft(10);
		snapSensitivityUI->setBounds(scr);

		cr.removeFromTop(2);
	}

	if (mappingOutputUI != nullptr)
	{
		mappingOutputUI->setBounds(cr.removeFromTop(14));
		cr.removeFromTop(2);
	}
	
	if(automationInspector != nullptr) automationInspector->setBounds(cr);
}

void MappingLayerPanel::newMessage(const Mapping::MappingEvent & e)
{
	switch (e.type)
	{
	case Mapping::MappingEvent::OUTPUT_TYPE_CHANGED:
		if (mappingOutputUI != nullptr)
		{
			removeChildComponent(mappingOutputUI.get());
			mappingOutputUI.reset(dynamic_cast<ParameterUI*>(mappingLayer->mapping.outputParam->createDefaultUI()));
			if (mappingOutputUI != nullptr) addAndMakeVisible(mappingOutputUI.get());
			resized();
		}
		break;
	}
}
