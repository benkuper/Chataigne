/*
  ==============================================================================

	PowerpointModule.cpp
	Created: 13 Jul 2018 12:11:43am
	Author:  Ben

  ==============================================================================
*/

ApplicationProperties& getAppProperties();

PowerPointModule::PowerPointModule() :
	OSCModule(getDefaultTypeString(), 35551, 35550)
{
	currentSlide = valuesCC.addIntParameter("Current Slide", "The current slide", 0, 0, INT32_MAX);
	totalSlides = valuesCC.addIntParameter("Total Slides", "The total number of slides", 0, 0, INT32_MAX);

	var indexArgs = var();
	var indexArg = ControllableUtil::createDataForParam(IntParameter::getTypeStringStatic(), "Slide Index", "Index of the slide", "", 1, INT32_MAX);
	indexArg.getDynamicObject()->setProperty("mappingIndex", 0);
	indexArgs.append(indexArg);

	defManager->add(CommandDefinition::createDef(this, "", "Next Slide", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/next"));
	defManager->add(CommandDefinition::createDef(this, "", "Previous Slide", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/previous"));
	defManager->add(CommandDefinition::createDef(this, "", "Go to Slide", &OSCCommand::create, CommandContext::BOTH)->addParam("address", "/slide")->addParam("args", indexArgs));

#if JUCE_WINDOWS
	installPowerPointPlugin();
#else
    setWarningMessage("This module only works on windows.");
#endif
}

PowerPointModule::~PowerPointModule()
{
}

void PowerPointModule::installPowerPointPlugin()
{
#if JUCE_WINDOWS
	String show = getAppProperties().getUserSettings()->getValue("showPowerpointInstallPlugin", "1");
	if (show == "1")
	{
		int result = AlertWindow::showYesNoCancelBox(AlertWindow::QuestionIcon, "Powerpoint OSC plugin not found", "The Powerpoint plugin is required to be able to actually use this module with Powerpoint.\n \
Do you want to install it ? Also, you need to have Powerpoint installed.", "Yes", "No", "Don't ask me again");

		if (result == 1)
		{
			URL ppURL("http://benjamin.kuperberg.fr/download/powerpoint-osc.zip");

			LOG("Downloading...");

			downloadedFile = File::getSpecialLocation(File::tempDirectory).getChildFile("PowerpointOSC.zip");
			downloadTask = ppURL.downloadToFile(downloadedFile, URL::DownloadTaskOptions().withListener(this));
			if (downloadTask == nullptr)
			{
				LOGERROR("Error downloading Powerpoint plugin, are you connected to internet ?");
				return;
			}
		}
		else if (result == 0)
		{
			getAppProperties().getUserSettings()->setValue("showPowerpointInstallPlugin", "0");
			LOG("Ok, I won't ask you again.");
		}
	}

#endif
}

void PowerPointModule::progress(URL::DownloadTask *, int64 bytesDownloaded, int64 totalBytes)
{
	if (totalBytes == -1) return;
	float p = bytesDownloaded * 1.0f / totalBytes;
	LOG("Downloading... " << (int)(p * 100) << "%");
}

void PowerPointModule::finished(URL::DownloadTask * task, bool success)
{
	if (!success)
	{
		LOGERROR("Error while downloading Powerpoint plugin.");
		return;
	}

	File pFolder = downloadedFile.getParentDirectory().getChildFile("PowerPointOSC");
	ZipFile zip(downloadedFile);
	zip.uncompressTo(pFolder);
	File installFile = pFolder.getChildFile("install.bat");
	if (!installFile.exists())
	{
		LOG("Could not find Powerpoint plugin installation file");
		return;
	}

	installFile.startAsProcess();

	AlertWindow::showMessageBox(AlertWindow::InfoIcon, "Powerpoint plugin installed", "The powerpoint plugin has been installed, you can now use the Powerpoint plugin !");
	getAppProperties().getUserSettings()->setValue("showPowerpointInstallPlugin", "0");
}

void PowerPointModule::processMessageInternal(const OSCMessage& m)
{
	String s = m.getAddressPattern().toString();
	if (s == "/currentSlide") currentSlide->setValue(m[0].getInt32());
	if (s == "/totalSlides") totalSlides->setValue(m[0].getInt32());
}
