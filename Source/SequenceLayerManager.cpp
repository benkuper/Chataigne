/*
  ==============================================================================

    SequenceLayerManager.cpp
    Created: 28 Oct 2016 8:15:28pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceLayerManager.h"

juce_ImplementSingleton(SequenceLayerManager)

SequenceLayerManager::SequenceLayerManager() :
	BaseManager<SequenceLayer>("SequenceLayers")
{
}

SequenceLayerManager::~SequenceLayerManager()
{
}
