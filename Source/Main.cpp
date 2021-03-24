#include "Main.h"

#if JUCE_MAC //for chmod
#include <sys/types.h>
#include <sys/stat.h>
#endif

//==============================================================================

ChataigneApplication::ChataigneApplication() :
	OrganicApplication("Chataigne", true, ImageCache::getFromMemory(BinaryData::tray_icon_png, BinaryData::tray_icon_pngSize))
{
	enableSendAnalytics = appSettings.addBoolParameter("Send Analytics", "This helps me improve the software by sending basic start/stop/crash infos", true);
}


void ChataigneApplication::initialiseInternal(const String &)
{
	engine.reset(new ChataigneEngine());
	mainComponent.reset(new MainContentComponent());

	//Call after engine init
	AppUpdater::getInstance()->setURLs("http://benjamin.kuperberg.fr/chataigne/releases/update.json", "http://benjamin.kuperberg.fr/chataigne/user/data/", "Chataigne");
	HelpBox::getInstance()->helpURL = URL("http://benjamin.kuperberg.fr/chataigne/help/");
	CrashDumpUploader::getInstance()->remoteURL = URL("http://benjamin.kuperberg.fr/chataigne/support/crash_report.php");
	CrashDumpUploader::getInstance()->crashImage = ImageCache::getFromMemory(BinaryData::crash_png, BinaryData::crash_pngSize);

	//DashboardManager::getInstance()->setupDownloadURL("http://benjamin.kuperberg.fr/download/dashboard/dashboard.php?folder=dashboard");

	ShapeShifterManager::getInstance()->setDefaultFileData(BinaryData::default_chalayout);
	ShapeShifterManager::getInstance()->setLayoutInformations("chalayout", "Chataigne/layouts");
}


void ChataigneApplication::afterInit()
{
	//ANALYTICS
	if (enableSendAnalytics->boolValue())
	{
		bool crashFound = CrashDumpUploader::getInstance()->crashFound;
		
		StringPairArray options;
		
		if (crashFound) MatomoAnalytics::getInstance()->log(MatomoAnalytics::CRASH); 
		else options.set("new_visit", "1");

		MatomoAnalytics::getInstance()->log(MatomoAnalytics::START, options);
	}

	DashboardManager::getInstance()->setupDownloadURL("http://benjamin.kuperberg.fr/download/dashboard/dashboard.php?folder=dashboard");

}

void ChataigneApplication::shutdown()
{   
	for (auto& m : ModuleManager::getInstance()->getItemsWithType<OSModule>())
	{
		m->terminateTrigger->trigger();
	}

	OrganicApplication::shutdown();

	if (enableSendAnalytics->boolValue())
	{
		MatomoAnalytics::getInstance()->log(MatomoAnalytics::STOP);
	}

	if(MatomoAnalytics::getInstanceWithoutCreating() != nullptr) MatomoAnalytics::deleteInstance();
	AppUpdater::deleteInstance();
}
