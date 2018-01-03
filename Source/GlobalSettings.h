/*
  ==============================================================================

    GlobalSettings.h
    Created: 3 Jan 2018 3:52:13pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class GlobalSettings :
	public ControllableContainer
{
public:
	juce_DeclareSingleton(GlobalSettings, true);

	GlobalSettings();
	~GlobalSettings();

	//Startup
	ControllableContainer startupCC;
	BoolParameter * checkUpdatesOnStartup;
	BoolParameter * checkBetaUpdates;
	BoolParameter * updateHelpOnStartup;
	BoolParameter * openLastDocumentOnStartup;

	ControllableContainer confirmationsCC;
	BoolParameter * askBeforeRemovingItems;
};