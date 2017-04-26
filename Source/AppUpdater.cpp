/*
  ==============================================================================

    AppUpdater.cpp
    Created: 8 Apr 2017 4:26:46pm
    Author:  Ben

  ==============================================================================
*/

#include "AppUpdater.h"
#include "DebugHelpers.h"
#include "Engine.h"

juce_ImplementSingleton(AppUpdater)

void AppUpdater::checkForUpdates()
{
	URL updateURL("http://benjamin.kuperberg.fr/chataigne/releases/update.json");
	URL downloadURL("http://benjamin.kuperberg.fr/chataigne");
	
	StringPairArray responseHeaders;
	int statusCode = 0; 
	ScopedPointer<InputStream> stream(updateURL.createInputStream(false, nullptr, nullptr, String(),
		200, // timeout in millisecs
		&responseHeaders, &statusCode));

	if (statusCode != 200) LOG("Failed to connect, status code = " + String(statusCode)); 
	else if (stream != nullptr)
	{
		String content = stream->readEntireStreamAsString();
		var data = JSON::parse(content);
		String version = data.getProperty("version", "");
		bool beta = data.getProperty("beta", false);
		Array<var> * changelog = data.getProperty("changelog", var()).getArray();
		
		if (Engine::getInstance()->checkFileVersion(data.getDynamicObject(), true))
		{
			String msg = "A new " + String(beta ? "beta " : "") + "version of Chataigne is available : " + version + "\nChangelog :\n";
			
			for (auto &c : *changelog) msg += c.toString() + "\n";
			msg += "Do you want to download it ?";

			int result = AlertWindow::showOkCancelBox(AlertWindow::InfoIcon, "New version available", msg, "Yes", "No");
			if (result) downloadURL.launchInDefaultBrowser();
		}
		else
		{
			LOG("App is up to date.");
		}
		

	}	
}
