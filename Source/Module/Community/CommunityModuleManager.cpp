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
	stopThread(5000);
}

void CommunityModuleManager::run()
{
	sleep(500);
	var data = getJSONDataForURL(URL("https://benjamin.kuperberg.fr/chataigne/releases/modules.json"));
	
	if (threadShouldExit()) return;

	if (!data.isObject())
	{
		LOGWARNING("Error fetching community modules, wrong data format.");
		return;
	}

	var defs = data.getProperty("definitions", var());
	
	if (!defs.isArray()) return;

	for (int i = 0; i < defs.size(); ++i)
	{
		if (threadShouldExit()) return;

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
	std::unique_ptr<InputStream> stream(url.createInputStream(false, openStreamProgressCallback, this, String(),
		2000, // timeout in millisecs
		&responseHeaders, &statusCode));

	if (threadShouldExit()) return var();

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
    }else{
        DBG("Error with url request : " << statusCode);
    }

	return var();
}

bool CommunityModuleManager::openStreamProgressCallback(void* context, int, int)
{
	auto thread = (CommunityModuleManager *)context;
	return !thread->threadShouldExit();
}
