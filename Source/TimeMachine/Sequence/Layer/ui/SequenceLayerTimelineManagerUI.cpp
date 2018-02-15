/*
  ==============================================================================

    SequenceLayerTimelineManagerUI.cpp
    Created: 20 Nov 2016 2:18:18pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceLayerTimelineManagerUI.h"
#include "../SequenceLayerFactory.h"

SequenceLayerTimelineManagerUI::SequenceLayerTimelineManagerUI(SequenceLayerManager * _manager) :
	BaseManagerUI<SequenceLayerManager, SequenceLayer, SequenceLayerTimeline>("Layers", _manager)
{
	addExistingItems();
}

SequenceLayerTimelineManagerUI::~SequenceLayerTimelineManagerUI()
{
}

SequenceLayerTimeline * SequenceLayerTimelineManagerUI::createUIForItem(SequenceLayer * layer)
{
	return layer->getTimelineUI();
}

void SequenceLayerTimelineManagerUI::resized()
{
	BaseManagerUI::resized();

}

void SequenceLayerTimelineManagerUI::showMenuAndAddItem(bool, Point<int>)
{
	SequenceLayer * i = SequenceLayerFactory::getInstance()->showCreateMenu(manager->sequence);
	if (i != nullptr) manager->addItem(i);
}
