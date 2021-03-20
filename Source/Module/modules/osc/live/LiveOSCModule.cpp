/*
  ==============================================================================

    LiveOSCModule.cpp
    Created: 13 Apr 2017 11:02:29am
    Author:  Ben

  ==============================================================================
*/

LiveOSCModule::LiveOSCModule() :
	OSCModule(getDefaultTypeString(), 11001, 11000, false, true)
{
	downloadMaxPatch = addTrigger("Download Max Patch", "This is necessary to make this plugin run. Just download the zip file, unzip it wherever you want and import the OSCControl.amxd into Live's master track (or any track actually.");

	defManager->add(CommandDefinition::createDef(this, "General", "Play", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("addressModel", "/live_set/start_playing"));
	defManager->add(CommandDefinition::createDef(this, "General", "Stop", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("addressModel", "/live_set/stop_playing"));
	defManager->add(CommandDefinition::createDef(this, "General", "Stop All Clips", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("addressModel", "/live_set/stop_all_clips"));
	defManager->add(CommandDefinition::createDef(this, "General", "Continue", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("addressModel", "/live_set/continue_playing"));

	defManager->add(CommandDefinition::createDef(this, "General", "Set Time", &LiveOSCCommandBase::create)
		->addParam("addressModel", "/live_set/current_song_time")->addParam("type", Controllable::FLOAT)->addParam("ui", FloatParameter::UIType::TIME)->addParam("min",0));

	defManager->add(CommandDefinition::createDef(this, "General", "Set Tempo", &LiveOSCCommandBase::create)->addParam("addressModel", "/live_set/tempo")->addParam("type", Controllable::FLOAT)->addParam("min", 20)->addParam("max", 999)->addParam("default", 120));
	defManager->add(CommandDefinition::createDef(this, "General", "Tap Tempo", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("addressModel", "/live_set/tap_tempo"));

	defManager->add(CommandDefinition::createDef(this, "General", "Master Volume", &LiveOSCCommandBase::create)
		->addParam("addressModel", "/live_set/master_track/mixer_device/volume/value")->addParam("type", Controllable::FLOAT)->addParam("min", 0)->addParam("max", 1)->addParam("default", 0.85f));

	defManager->add(CommandDefinition::createDef(this, "Scene", "Play Scene", &LiveOSCCommandBase::create)->addParam("addressModel", "/live_set/scenes/[scene]/fire"));

	defManager->add(CommandDefinition::createDef(this, "Track", "Set Volume", &LiveOSCCommandBase::create)
		->addParam("addressModel", "/live_set/tracks/[track]/mixer_device/volume/value")->addParam("type",Controllable::FLOAT)->addParam("min", 0)->addParam("max", 1)->addParam("default",0.85f));

	defManager->add(CommandDefinition::createDef(this, "Track", "Set Pan", &LiveOSCCommandBase::create)
		->addParam("addressModel", "/live_set/tracks/[track]/mixer_device/panning/value")->addParam("type", Controllable::FLOAT)->addParam("min", -1)->addParam("max", 1)); 
	
	defManager->add(CommandDefinition::createDef(this, "Track", "Set Mute", &LiveOSCCommandBase::create)->addParam("addressModel", "/live_set/tracks/[track]/mute")->addParam("type",Controllable::BOOL));
	defManager->add(CommandDefinition::createDef(this, "Track", "Set Armed", &LiveOSCCommandBase::create)->addParam("addressModel", "/live_set/tracks/[track]/arm")->addParam("type",Controllable::BOOL));
	
	defManager->add(CommandDefinition::createDef(this, "Track", "Set Solo", &LiveOSCCommandBase::create)->addParam("addressModel", "/live_set/tracks/[track]/solo")->addParam("type",Controllable::BOOL));
	//defManager->add(CommandDefinition::createDef(this, "Track", "Set Send", &LiveOSCCommandBase::create)->addParam("type",Controllable::FLOAT));
	defManager->add(CommandDefinition::createDef(this, "Track", "Stop Track Clips", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("addressModel", "/live_set/tracks/[track]/stop_all_clips"));

	defManager->add(CommandDefinition::createDef(this, "Device", "Set Enabled", &LiveOSCCommandBase::create)->addParam("addressModel", "/live_set/tracks/[track]/devices/[device]/parameters/0/value")->addParam("type", Controllable::BOOL));

	defManager->add(CommandDefinition::createDef(this, "Device", "Set Parameter", &LiveOSCCommandBase::create)->addParam("addressModel", "/live_set/tracks/[track]/devices/[device]/parameters/[parameter]/value")->addParam("type", Controllable::FLOAT));

	//detectAndInstallLivePlugin();
}

void LiveOSCModule::onContainerTriggerTriggered(Trigger* t)
{
	OSCModule::onContainerTriggerTriggered(t);
	if (t == downloadMaxPatch) URL("http://benjamin.kuperberg.fr/download/LiveOSCControl.zip").launchInDefaultBrowser();
}

/*
void LiveOSCModule::detectAndInstallLivePlugin()
{
#if JUCE_WINDOWS || JUCE_MAC

	
#if JUCE_WINDOWS
	String appPath = "C:/ProgramData/Ableton";
	String remoteScriptsFolder = "Resources/MIDI Remote Scripts/LiveOSC2";
#elif JUCE_MAC
	String appPath = "/Applications";
	String remoteScriptsFolder = "Contents/App-Resources/MIDI Remote Scripts/LiveOSC2";
#endif


	File appFolder(appPath);
	if(appFolder.exists())
	{
		Array<File> files = appFolder.findChildFiles(File::TypesOfFileToFind::findDirectories, false, "*Live*");
		for (auto &f : files)
		{
			if (f.getFileName().contains(".")) continue;
			liveFolder = f;
			LOG("Found " << f.getFileName());
			break;
		}
	}


	if (liveFolder.exists())
	{
		pluginFolder = liveFolder.getChildFile(remoteScriptsFolder);

		LOG("Live installation detected in :\n" << liveFolder.getFullPathName());

		if (pluginFolder.exists())
		{
			LOG("LiveOSC2 plugin found. Have fun !");
			return;
		}
		else
		{
			LOGWARNING("LiveOSC2 plugin not found.");
		}
	}
	else
	{
		LOGWARNING("No Live installation not detected in standard path, are you using special location ?");
	}

	int result = AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "LiveOSC plugin not found", "The LiveOSC2 plugin is required to be able to actually use this module with Ableton Live.\n \
Do you want to install it ?", "Yes", "No");

	if (result)
	{
		URL liveOSCURL("https://github.com/stufisher/LiveOSC2/archive/master.zip");
		
		LOG("Downloading...");

		downloadedFile = File::getSpecialLocation(File::tempDirectory).getChildFile("LiveOSC2.zip");
		downloadTask = liveOSCURL.downloadToFile(downloadedFile, "", this);
		if (downloadTask == nullptr)
		{
			LOGERROR("Error downloading LiveOSC2 plugin, are you connected to internet ?");
			return;
		}
	}

#endif
}

void LiveOSCModule::progress(URL::DownloadTask *, int64 bytesDownloaded, int64 totalBytes)
{
	if (totalBytes == -1) return;
	float p = bytesDownloaded * 1.0f / totalBytes;
	LOG("Downloading... " << (int)(p*100) << "%");
}

void LiveOSCModule::finished(URL::DownloadTask * task, bool success)
{
	if (!success)
	{
		LOGERROR("Error while downloading LiveOSC2.");
		return;
	}

	File scriptsFolder = pluginFolder.getParentDirectory();

	if (!scriptsFolder.exists())
	{
		LOGERROR("Live installation not found");
		return;
	}

	ZipFile zip(downloadedFile);
	zip.uncompressTo(scriptsFolder);
	scriptsFolder.getChildFile("LiveOSC2-master").moveFileTo(scriptsFolder.getChildFile("LiveOSC2"));

	AlertWindow::showMessageBox(AlertWindow::InfoIcon, "LiveOSC2 plugin installed", "The LiveOSC2 plugin is now installed.\n \
You will need to restart Ableton Live and select \"LiveOSC2\" as a Control Surface in Ableton Live Preferences (MIDI menu)\nEnjoy !");

}

*/