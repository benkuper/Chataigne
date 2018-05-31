#include "Main.h"

#if JUCE_MAC //for chmod
#include <sys/types.h>
#include <sys/stat.h>
#endif

//==============================================================================

ChataigneApplication::ChataigneApplication() :
	OrganicApplication("Chataigne")
{
	enableSendAnalytics = appSettings.addBoolParameter("Send Analytics", "This helps me improve the software by sending basic start/stop/crash infos", true);
}


void ChataigneApplication::initialiseInternal(const String &)
{
	AppUpdater::getInstance()->setURLs(URL("http://benjamin.kuperberg.fr/chataigne/releases/update.json"), "http://benjamin.kuperberg.fr/chataigne/user/data/", "Chataigne");
	HelpBox::getInstance()->helpURL = URL("http://benjamin.kuperberg.fr/chataigne/docs/help.json");

	engine = new ChataigneEngine(appProperties, getAppVersion());
	mainComponent = new MainContentComponent();


	ShapeShifterManager::getInstance()->setDefaultFileData(BinaryData::default_chalayout);
	ShapeShifterManager::getInstance()->setLayoutInformations("chalayout", "Chataigne/layouts");

	//ANALYTICS
	if (enableSendAnalytics->boolValue())
	{
		Analytics::getInstance()->setUserId(SystemStats::getFullUserName());

		// Add any analytics destinations we want to use to the Analytics singleton.
		Analytics::getInstance()->addDestination(new GoogleAnalyticsDestination());
		Analytics::getInstance()->logEvent("startup", {});
	}
}

void ChataigneApplication::shutdown()
{   
	OrganicApplication::shutdown();

	if (enableSendAnalytics->boolValue()) Analytics::getInstance()->logEvent("shutdown", {});
	AppUpdater::deleteInstance();
}
