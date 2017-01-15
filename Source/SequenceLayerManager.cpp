/*
  ==============================================================================

    SequenceLayerManager.cpp
    Created: 28 Oct 2016 8:15:28pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerManager.h"
#include "SequenceLayerFactory.h"

SequenceLayerManager::SequenceLayerManager(Sequence * _sequence) :
	BaseManager<SequenceLayer>("Layers"),
	sequence(_sequence)
{
	skipControllableNameInAddress = true;
}

SequenceLayerManager::~SequenceLayerManager()
{
}

SequenceLayer * SequenceLayerManager::createItem()
{
	return new SequenceLayer(sequence);
}

void SequenceLayerManager::addItemFromData(var data)
{
	String layerType = data.getProperty("type", "none");
	if (layerType.isEmpty()) return;
	SequenceLayer * i = SequenceLayerFactory::getInstance()->createSequenceLayer(sequence,layerType);
	if (i != nullptr) addItem(i, data);
	else
	{
		
	}
}
