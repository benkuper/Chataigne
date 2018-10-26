/*
  ==============================================================================

    SequenceLayerPanelManagerUI.cpp
    Created: 28 Oct 2016 8:15:41pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerPanelManagerUI.h"
#include "../SequenceLayerFactory.h"

SequenceLayerPanelManagerUI::SequenceLayerPanelManagerUI(SequenceLayerManager * _manager) :
	BaseManagerUI<SequenceLayerManager, SequenceLayer, SequenceLayerPanel>("SequenceLayers", _manager)
{
	viewport.setScrollBarsShown(false, false, true, false);
	addExistingItems();
}

SequenceLayerPanelManagerUI::~SequenceLayerPanelManagerUI()
{
}


void SequenceLayerPanelManagerUI::showMenuAndAddItem(bool /*isFromAddButton*/, Point<int>)
{
	SequenceLayer * i = SequenceLayerFactory::getInstance()->showCreateMenu(manager->sequence);
	if (i != nullptr) manager->addItem(i);
}

SequenceLayerPanel * SequenceLayerPanelManagerUI::createUIForItem(SequenceLayer * layer)
{
	return layer->getPanel();
}
