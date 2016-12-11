/*
  ==============================================================================

    SequenceLayerManager.cpp
    Created: 28 Oct 2016 8:15:28pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerManager.h"

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