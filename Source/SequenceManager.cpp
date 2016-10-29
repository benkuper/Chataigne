/*
  ==============================================================================

    SequenceManager.cpp
    Created: 28 Oct 2016 8:13:04pm
    Author:  bkupe

  ==============================================================================
*/

#include "SequenceManager.h"

juce_ImplementSingleton(SequenceManager)

SequenceManager::SequenceManager() :
	BaseManager<Sequence>("Sequences")
{
}

SequenceManager::~SequenceManager()
{
}
