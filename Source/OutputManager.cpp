/*
  ==============================================================================

    OutputManager.cpp
    Created: 28 Oct 2016 8:02:49pm
    Author:  bkupe

  ==============================================================================
*/

#include "OutputManager.h"

juce_ImplementSingleton(OutputManager)

OutputManager::OutputManager() :
	BaseManager<Output>("Outputs")
{
}

OutputManager::~OutputManager()
{
}
