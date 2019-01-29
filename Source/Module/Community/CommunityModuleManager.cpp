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
	startThread();
}

CommunityModuleManager::~CommunityModuleManager()
{

}

void CommunityModuleManager::run()
{
	URL modulesURL = "http://benjamin.kuperberg.fr/chataigne/releases/modules.json";

	StringPairArray responseHeaders;
	int statusCode = 0;
	ScopedPointer<InputStream> stream(modulesURL.createInputStream(false, nullptr, nullptr, String(),
		2000, // timeout in millisecs
		&responseHeaders, &statusCode));
#if JUCE_WINDOWS
	if (statusCode != 200)
	{
		LOGWARNING("Failed to connect, status code = " + String(statusCode));
		return;
	}
#endif

	DBG("CommunityModule:: Status code " << statusCode);

	if (stream != nullptr)
	{
		String content = stream->readEntireStreamAsString();
		var data = JSON::parse(content);

		if (data.isObject())
		{
			NamedValueSet mList = data.getDynamicObject()->getProperties();
			for (auto &mData : mList)
			{
				var localData = ModuleFactory::getInstance()->getCustomModuleInfo(mData.name);
				File localFolder = File();
				if (!localData.isVoid()) localFolder = ModuleFactory::getInstance()->getFolderForCustomModule(mData.name);
				CommunityModuleInfo * m = new CommunityModuleInfo(mData.name, mData.value, localData, localFolder);
				addItem(m);
			}
		}
	}
	
}