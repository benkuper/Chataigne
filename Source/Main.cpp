#include "Main.h"

#if JUCE_MAC //for chmod
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include "Module/ModuleIncludes.h"

//==============================================================================

ChataigneApplication::ChataigneApplication() :
	OrganicApplication("Chataigne", true, ImageCache::getFromMemory(BinaryData::tray_icon_png, BinaryData::tray_icon_pngSize)),
    crashSent(false)
{
	enableSendAnalytics = appSettings.addBoolParameter("Send Analytics", "This helps me improve the software by sending basic start/stop/crash infos", true);
}


void ChataigneApplication::initialiseInternal(const String &)
{
   #if JUCE_LINUX
	// The launch script sets LD_LIBRARY_PATH so this binary can find its own bundled
	// libraries, but that same variable leaks into every child process this app spawns
	// (e.g. zenity/kdialog for native file choosers), making them load outdated bundled
	// libs instead of the system's own and crash with symbol lookup errors. Our own
	// executable and its directly-linked libraries are already resolved by the dynamic
	// loader before main() runs, so clearing it here only affects processes we spawn
	// from this point on.
	unsetenv("LD_LIBRARY_PATH");
   #endif

	engine.reset(new ChataigneEngine());
	if(useWindow) mainComponent.reset(new MainContentComponent());

	//Call after engine init
	AppUpdater::getInstance()->setURLs("http://benjamin.kuperberg.fr/chataigne/releases/update.json", "http://benjamin.kuperberg.fr/chataigne/user/data/", "Chataigne");
	HelpBox::getInstance()->helpURL = URL("http://benjamin.kuperberg.fr/chataigne/help/");

	CrashDumpUploader::getInstance()->init("http://benjamin.kuperberg.fr/chataigne/support/crash_report.php",ImageCache::getFromMemory(BinaryData::crash_png, BinaryData::crash_pngSize));

	//DashboardManager::getInstance()->setupDownloadURL("http://benjamin.kuperberg.fr/download/dashboard/dashboard.php?folder=dashboard");

	ShapeShifterManager::getInstance()->setDefaultFileData(BinaryData::default_chalayout);
	ShapeShifterManager::getInstance()->setLayoutInformations("chalayout", "Chataigne/layouts");

	// Set default font to support Chinese characters
	// On macOS, use PingFang SC which supports Chinese
	// On Windows, use Microsoft YaHei which supports Chinese
	#if JUCE_MAC
		LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("PingFang SC");
	#elif JUCE_WINDOWS
		LookAndFeel::getDefaultLookAndFeel().setDefaultSansSerifTypefaceName("Microsoft YaHei");
	#endif

}


void ChataigneApplication::afterInit()
{
	//ANALYTICS
	if (!launchedFromCrash && enableSendAnalytics->boolValue())
	{
		StringPairArray options;
		options.set("new_visit", "1");
		MatomoAnalytics::getInstance()->log(MatomoAnalytics::START, options);
	}

	DashboardManager::getInstance()->setupDownloadURL("http://benjamin.kuperberg.fr/download/dashboard/dashboard.php?folder=dashboard");

	if (mainWindow != nullptr)
	{
		mainWindow->setMenuBarComponent(new ChataigneMenuBarComponent((MainContentComponent*)mainComponent.get(), (ChataigneEngine*)engine.get()));
	}

}

void ChataigneApplication::shutdown()
{   
	if (isInitialising()) return;
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

void ChataigneApplication::handleCrashed()
{
	for (auto& m : ModuleManager::getInstance()->getItemsWithType<OSModule>())
	{
		m->crashedTrigger->trigger();
	}
    
	if (!crashSent && !launchedFromCrash && enableSendAnalytics->boolValue())
	{
        crashSent = true;
		MatomoAnalytics::getInstance()->log(MatomoAnalytics::CRASH);
		MatomoAnalytics::getInstance()->signalThreadShouldExit();
		while (MatomoAnalytics::getInstance()->isThreadRunning())
		{
			//wait until thread is done
			Thread* t = Thread::getCurrentThread();
			if (t == nullptr) break;
			t->wait(10);
        }
	}

	OrganicApplication::handleCrashed();
}
