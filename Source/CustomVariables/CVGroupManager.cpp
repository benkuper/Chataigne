/*
  ==============================================================================

    CVGroupManager.cpp
    Created: 17 Feb 2018 10:16:11am
    Author:  Ben

  ==============================================================================
*/

#include "CVGroupManager.h"

juce_ImplementSingleton(CVGroupManager)

CVGroupManager::CVGroupManager(const String & name) :
	BaseManager(name)
{

}

CVGroupManager::~CVGroupManager()
{

}
