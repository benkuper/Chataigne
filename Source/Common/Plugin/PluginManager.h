/*
  ==============================================================================

    PluginManager.h
    Created: 27 Dec 2016 2:36:02pm
    Author:  Ben

  ==============================================================================
*/

#ifndef PLUGINMANAGER_H_INCLUDED
#define PLUGINMANAGER_H_INCLUDED

#include "JuceHeader.h"

class PluginManager
{
public:
	juce_DeclareSingleton(PluginManager,false)
	PluginManager();
	~PluginManager();

	void detectPlugins();
};



#endif  // PLUGINMANAGER_H_INCLUDED
