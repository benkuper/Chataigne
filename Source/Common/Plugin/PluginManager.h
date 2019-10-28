/*
  ==============================================================================

    PluginManager.h
    Created: 27 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class PluginManager
{
public:
	juce_DeclareSingleton(PluginManager,false)
	PluginManager();
	~PluginManager();

	void detectPlugins();
};