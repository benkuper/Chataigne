/*
  ==============================================================================

	CommunityModulePanel.cpp
	Created: 22 Jun 2024 5:29:38pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "CommunityModulePanel.h"

CommunityModulePanel::CommunityModulePanel(const String& name) :
	ShapeShifterContentComponent(name)
{
	var initInfo = ModuleManager::getInstance()->factory->getCustomModulesInfo();
	webComp.reset(new WebComp(initInfo));
	webComp->modulesURL = "https://benjamin.kuperberg.fr/chataigne/modules_browser/"; // http://localhost:5173/";
	webComp->goToURL(webComp->modulesURL + "?inApp=1");
	addAndMakeVisible(webComp.get());
}

CommunityModulePanel::~CommunityModulePanel()
{
}

void CommunityModulePanel::resized()
{
	webComp->setBounds(getLocalBounds());
}

void CommunityModulePanel::mouseDown(const MouseEvent& e)
{

}


CommunityModulePanel::WebComp::WebComp(var initData)
	: WebBrowserComponent(WebBrowserComponent::Options().withBackend(WebBrowserComponent::Options::Backend::webview2).withKeepPageLoadedWhenBrowserIsHidden()
		.withInitialisationData("localModuleData", initData).withNativeIntegrationEnabled()
		.withNativeFunction("install", [](auto& data, auto complete)
			{
				if (data.size() == 0) return;


				var moduleInfo = JSON::parse(data[0]);
				String moduleName = moduleInfo["name"];
				String downloadURL = moduleInfo["downloadURL"];
				LOG("Installing from module browser " << moduleName << " ( " << downloadURL << " )");

				bool result = CommunityModuleManager::getInstance()->installModule(downloadURL, moduleName);
				complete(result);
			}
		).withNativeFunction("setInfo", [](auto& data, auto complete)
			{
				if (data.size() == 0) return;

				var moduleInfo = JSON::parse(data[0]);
				String moduleName = moduleInfo["name"];
				String version = moduleInfo["version"];

				int status = 0; //0 = not installed, 1 = installed and up to date, 2 = update available
				var moduleData = ModuleManager::getInstance()->factory->getCustomModuleInfo(moduleName);
				String localVersion = moduleData.getProperty("version", "0");
				if (moduleData.isVoid() || localVersion == "0")
				{
					status = 0;
				}else if (version > localVersion)
				{
					status = 2;
					ModuleManager::getInstance()->factory->setModuleNewVersionAvailable(moduleName, true);
				}
				else
				{
					status = 1;
				}

				complete(status);
			})
	)
{

}

bool CommunityModulePanel::WebComp::pageAboutToLoad(const String& url) {
	if (url.startsWith(modulesURL)) return true;
	else if(url.startsWith("http")) URL(url).launchInDefaultBrowser();
	return false;
}

void CommunityModulePanel::WebComp::pageFinishedLoading(const String& url) {
	//LOG("Finished loading : " << url);
}

bool CommunityModulePanel::WebComp::pageLoadHadNetworkError(const String& error) {
	//LOG("Error loading welcome : " << error);
	return true;
}
