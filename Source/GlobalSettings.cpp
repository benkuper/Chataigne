/*
  ==============================================================================

    GlobalSettings.cpp
    Created: 3 Jan 2018 3:52:13pm
    Author:  Ben

  ==============================================================================
*/

#include "GlobalSettings.h"

juce_ImplementSingleton(GlobalSettings)

GlobalSettings::GlobalSettings() :
	ControllableContainer("Global Settings"),
	startupCC("Startup and Update"),
	confirmationsCC("Confirmation messages")
{
	saveAndLoadRecursiveData = true;

	addChildControllableContainer(&startupCC);
	checkUpdatesOnStartup = startupCC.addBoolParameter("Check updates on startup", "If enabled, app will check if any updates are available",true);
	checkBetaUpdates = startupCC.addBoolParameter("Check for beta updates", "If enabled the app will also check for beta versions of the software", true);
	updateHelpOnStartup = startupCC.addBoolParameter("Update help on startup", "If enabled, app will try and download the last help file locally", true);
	openLastDocumentOnStartup = startupCC.addBoolParameter("Load last opened document on startup", "If enabled, app will load the last document on startup", false);

	addChildControllableContainer(&confirmationsCC);
	//askBeforeRemovingItems = confirmationsCC.addBoolParameter("Ask before removing items", "If enabled, you will get a confirmation prompt before removing any item", true);
}

GlobalSettings::~GlobalSettings()
{
}
