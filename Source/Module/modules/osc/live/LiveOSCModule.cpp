/*
  ==============================================================================

    LiveOSCModule.cpp
    Created: 13 Apr 2017 11:02:29am
    Author:  Ben

  ==============================================================================
*/

#include "LiveOSCModule.h"
#include "commands/LiveOSCSceneCommand.h"
#include "commands/LiveOSCCommandBase.h"

LiveOSCModule::LiveOSCModule() :
	OSCModule(getDefaultTypeString(), 9000, 9001, false, true)
{
	defManager.add(CommandDefinition::createDef(this, "General", "Play", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/live/play"));
	defManager.add(CommandDefinition::createDef(this, "General", "Continue", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/live/play/continue"));
	defManager.add(CommandDefinition::createDef(this, "General", "Stop", &OSCCommand::create, CommandContext::ACTION)->addParam("address", "/live/stop"));

	var floatValueArgs = var();
	var floatArg = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Tempo", "Target Tempo", 0, 20,999);
	floatArg.getDynamicObject()->setProperty("mappingIndex", 0);
	floatValueArgs.append(floatArg);
	defManager.add(CommandDefinition::createDef(this, "General", "Set Tempo", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/live/tempo")->addParam("args", floatValueArgs));
	
	floatValueArgs = var();
	floatArg = ControllableUtil::createDataForParam(FloatParameter::getTypeStringStatic(), "Time", "Target Time", 0, 0, 600);
	floatArg.getDynamicObject()->setProperty("mappingIndex", 0);
	floatValueArgs.append(floatArg); 
	defManager.add(CommandDefinition::createDef(this, "General", "Set Time", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/live/time")->addParam("args",floatValueArgs));
	defManager.add(CommandDefinition::createDef(this, "Scene", "Play Scene", &LiveOSCSceneCommand::create, CommandContext::ACTION));

	defManager.add(CommandDefinition::createDef(this, "Track", "Set Volume", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::VOLUME));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Mute", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::MUTE));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Armed", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::ARM));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Pan", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::PAN));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Solo", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::SOLO));
	defManager.add(CommandDefinition::createDef(this, "Track", "Set Send", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::SEND));
	defManager.add(CommandDefinition::createDef(this, "Track", "Stop", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("type", LiveOSCCommandBase::TRACK_STOP));

	defManager.add(CommandDefinition::createDef(this, "Device", "Set Enabled", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::DEVICE_ENABLE));
	defManager.add(CommandDefinition::createDef(this, "Device", "Set Parameter", &LiveOSCCommandBase::create, CommandContext::BOTH)->addParam("type", LiveOSCCommandBase::DEVICE_PARAM));

	defManager.add(CommandDefinition::createDef(this, "Clip", "Play Clip", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("type", LiveOSCCommandBase::CLIP_PLAY));
	defManager.add(CommandDefinition::createDef(this, "Clip", "Stop Clip", &LiveOSCCommandBase::create, CommandContext::ACTION)->addParam("type", LiveOSCCommandBase::CLIP_STOP));

	detectAndInstallLivePlugin();
}

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
		downloadTask.reset(liveOSCURL.downloadToFile(downloadedFile, "", this));
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
