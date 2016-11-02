/*
  ==============================================================================

    MappingFilterManager.cpp
    Created: 28 Oct 2016 8:08:47pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingFilterManager.h"

juce_ImplementSingleton(MappingFilterManager)

MappingFilterManager::MappingFilterManager() :
	BaseManager<MappingFilter>("Filters")
{
}

MappingFilterManager::~MappingFilterManager()
{
}