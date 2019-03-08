/*
  ==============================================================================

    CommunityModuleManager.cpp
    Created: 29 Jan 2019 3:52:46pm
    Author:  bkupe

  ==============================================================================
*/

#include "CommunityModuleManager.h"
#include "Module/ModuleFactory.h"

juce_ImplementSingleton(CommunityModuleManager)

CommunityModuleManager::CommunityModuleManager() :
	BaseManager("Community Modules"),
	Thread("communityModules")
{
	userCanAddControllables = false;
	userCanAddItemsManually = false;
    selectItemWhenCreated = false;
	startThread();
}

CommunityModuleManager::~CommunityModuleManager()
{
	waitForThreadToExit(2000);
}

void CommunityModuleManager::run()
{
	var data = getJSONDataForURL(URL("http://benjamin.kuperberg.fr/chataigne/releases/modules.json"));
	
	if (!data.isObject())
	{
		LOGWARNING("Error fetching community modules, wrong data format.");
		return;
	}

	var defs = data.getProperty("definitions", var());
	
	if (!defs.isArray()) return;

	for (int i = 0; i < defs.size(); i++)
	{
		String defURL = defs[i].toString();
		if (defURL.isEmpty()) continue;

		var moduleDefData = getJSONDataForURL(URL(defURL));

		if (!moduleDefData.isObject())
		{
			LOGWARNING("Community Module definition file is not valid : " << defURL);
			continue;
		}

		String moduleName = moduleDefData.getProperty("name", "");

		if (moduleName.isEmpty()) {
			DBG("Module name is empty !");
			continue;
		}

		CommunityModuleInfo * m = new CommunityModuleInfo(moduleName, moduleDefData);
		addItem(m, var(), false);
		
	}
	
}

var CommunityModuleManager::getJSONDataForURL(URL url)
{
	
	StringPairArray responseHeaders;
	int statusCode = 0;
	ScopedPointer<InputStream> stream(url.createInputStream(false, nullptr, nullptr, String(),
		2000, // timeout in millisecs
		&responseHeaders, &statusCode));
#if JUCE_WINDOWS
	if (statusCode != 200)
	{
		LOGWARNING("Failed to connect, status code = " + String(statusCode));
		return var();
	}
#endif


	if (stream != nullptr)
	{
		String content = stream->readEntireStreamAsString();
		var data = JSON::parse(content);
		return data;
	}

	return var();
}
