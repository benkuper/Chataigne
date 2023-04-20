/*
  ==============================================================================

    CommunityModuleManager.cpp
    Created: 29 Jan 2019 3:52:46pm
    Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(CommunityModuleManager)

CommunityModuleManager::CommunityModuleManager() :
	BaseManager("Community Modules"),
	Thread("communityModules")
{
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
	wait(500);

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
	
	LOG("Finished fetching Community modules.");

	bool showUpdate = false;
	for (auto& i : items)
	{
		if (i->status == CommunityModuleInfo::NEW_VERSION_AVAILABLE)
		{
			showUpdate = true;
			break;
		}
	}

	if (showUpdate)
	{
		LOG("Some Community Modules have an update available. Quick quick, check it out !");
	}
}

var CommunityModuleManager::getJSONDataForURL(URL url)
{
	StringPairArray responseHeaders;
	int statusCode = 0;

	std::unique_ptr<InputStream> stream(url.createInputStream(
		URL::InputStreamOptions(URL::ParameterHandling::inAddress)
		.withExtraHeaders("Cache-Control: no-cache")
		.withConnectionTimeoutMs(2000)
		.withProgressCallback(std::bind(&CommunityModuleManager::openStreamProgressCallback, this, std::placeholders::_1, std::placeholders::_2))
		.withResponseHeaders(&responseHeaders)
		.withStatusCode(&statusCode)
	));

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

CommunityModuleInfo* CommunityModuleManager::getModuleInfoForFolder(const File &folder)
{
	for (auto& i : items) if (i->localModuleFolder == folder) return i;
	return nullptr;
}

bool CommunityModuleManager::openStreamProgressCallback(int, int)
{
	return !threadShouldExit();
}
