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
	openLastDocumentOnStartup = startupCC.addBoolParameter("Load last noisette on startup", "If enabled, app will load the last noisette on startup", false);
	openSpecificFileOnStartup = startupCC.addBoolParameter("Load specific noisette on startup", "If enabled, app will load the noisette specified below on startup", false,false);
	fileToOpenOnStartup = startupCC.addStringParameter("File to load on startup", "File to load when start, if the option above is checked", "", false);
	fileToOpenOnStartup->defaultUI = StringParameter::FILE;

	addChildControllableContainer(&confirmationsCC);
	//askBeforeRemovingItems = confirmationsCC.addBoolParameter("Ask before removing items", "If enabled, you will get a confirmation prompt before removing any item", true);
	askForSaveBeforeClosing = confirmationsCC.addBoolParameter("Always ask to save before closing", "If enabled, the app will always prompt to save the current file before closing", true);

}

GlobalSettings::~GlobalSettings()
{
}

void GlobalSettings::controllableFeedbackUpdate(ControllableContainer *, Controllable * c)
{
	if (c == openLastDocumentOnStartup)
	{
		if (openLastDocumentOnStartup->boolValue()) openSpecificFileOnStartup->setValue(false);
		openSpecificFileOnStartup->setEnabled(!openLastDocumentOnStartup->boolValue());
	}
	if (c == openLastDocumentOnStartup || c == openSpecificFileOnStartup)
	{
		fileToOpenOnStartup->setEnabled(openSpecificFileOnStartup->boolValue());
	}
}

void GlobalSettings::loadJSONDataInternal(var data)
{
	openSpecificFileOnStartup->setEnabled(!openLastDocumentOnStartup->boolValue());
	fileToOpenOnStartup->setEnabled(openSpecificFileOnStartup->boolValue());
}
