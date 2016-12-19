/*
  ==============================================================================

    MappingOutputManager.cpp
    Created: 28 Oct 2016 8:11:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingOutputManager.h"

juce_ImplementSingleton(MappingOutputManager)

MappingOutputManager::MappingOutputManager() :
	BaseManager<MappingOutput>("Outputs")
{
	hideInEditor = false;
}

MappingOutputManager::~MappingOutputManager()
{
}
