/*
  ==============================================================================

    ActionManager.cpp
    Created: 28 Oct 2016 8:05:29pm
    Author:  bkupe

  ==============================================================================
*/

#include "ActionManager.h"


juce_ImplementSingleton(ActionManager)

ActionManager::ActionManager() :
	BaseManager<Action>("Actions")
{
}

ActionManager::~ActionManager()
{
}
