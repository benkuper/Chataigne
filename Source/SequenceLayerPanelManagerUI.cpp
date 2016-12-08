/*
  ==============================================================================

    SequenceLayerPanelManagerUI.cpp
    Created: 28 Oct 2016 8:15:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerPanelManagerUI.h"
#include "SequenceLayerFactory.h"

SequenceLayerPanelManagerUI::SequenceLayerPanelManagerUI(SequenceLayerManager * _manager) :
	BaseManagerUI<SequenceLayerManager, SequenceLayer, SequenceLayerPanel>("SequenceLayers", _manager)
{
	useDefaultMenu = false;
}

SequenceLayerPanelManagerUI::~SequenceLayerPanelManagerUI()
{
}


void SequenceLayerPanelManagerUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if (e.mods.isRightButtonDown())
	{
		SequenceLayer * i = SequenceLayerFactory::getInstance()->showCreateMenu();
		if (i != nullptr) manager->addItem(i);
	}
}

SequenceLayerPanel * SequenceLayerPanelManagerUI::createUIForItem(SequenceLayer * layer)
{
	return layer->getPanel();
}
