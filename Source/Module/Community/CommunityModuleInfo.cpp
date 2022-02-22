/*
  ==============================================================================

	CommunityModuleInfo.cpp
	Created: 29 Jan 2019 3:52:31pm
	Author:  bkupe

  ==============================================================================
*/

CommunityModuleInfo::CommunityModuleInfo(StringRef name, var onlineData) :
	BaseItem(name, false),
	status(NOT_INSTALLED)
{
	nameCanBeChangedByUser = false;
	userCanRemove = false;
	canBeReorderedInEditor = false;
	editorCanBeCollapsed = true;
	editorIsCollapsed = true;

	installTriger = addTrigger("Install", "Install or Update this module");
	uninstallTrigger = addTrigger("Uninstall", "Uninstall this module");

	isOnline = addBoolParameter("Is Online", "Is this module registered online ?", !onlineData.isVoid());
	isLocal = addBoolParameter("Is Local", "Is this module installed locally ?", false);

	if (!onlineData.isVoid())
	{
		url = onlineData.getProperty("url", "");
		description = onlineData.getProperty("description", "");
		downloadURL = onlineData.getProperty("downloadURL", "");
		onlineVersion = onlineData.getProperty("version", "Unknown");

	}

	updateLocalData();

}

CommunityModuleInfo::~CommunityModuleInfo()
{
}

void CommunityModuleInfo::installModule()
{
	DBG("Download to file " << downloadURL << " > " << getDownloadFilePath().getFullPathName());
	File df = getDownloadFilePath();
	if (df.exists()) df.deleteFile();
	downloadTask = nullptr;
	downloadTask = URL(downloadURL).downloadToFile(df, URL::DownloadTaskOptions().withListener(this));
	if (downloadTask == nullptr)
	{
		LOGERROR("Error trying to download module " + niceName);
	}
}

File CommunityModuleInfo::getDownloadFilePath()
{
	File tmpFolder = File::getSpecialLocation(File::tempDirectory).getChildFile("Chataigne");
	if (!tmpFolder.exists()) tmpFolder.createDirectory();
	return tmpFolder.getChildFile(niceName + ".zip");
}

void CommunityModuleInfo::updateLocalData()
{
	var localData = ModuleManager::getInstance()->factory->getCustomModuleInfo(niceName);

	localVersion = localData.getProperty("version", "Unknown");
	localModuleFolder = ModuleManager::getInstance()->factory->getFolderForCustomModule(niceName);

	if (localData.isVoid())
	{
		status = NOT_INSTALLED;
	}
	else
	{
		if (onlineVersion > localVersion)
		{
			status = NEW_VERSION_AVAILABLE;
		}
		else if (onlineVersion == localVersion)
		{
			status = UP_TO_DATE;
		}
		else
		{
			status = INSTALLED;
		}

		ModuleManager::getInstance()->factory->setModuleNewVersionAvailable(niceName, status == NEW_VERSION_AVAILABLE);
	}

	installTriger->setEnabled(status != UP_TO_DATE);
	isLocal->setValue(status != NOT_INSTALLED);
}

void CommunityModuleInfo::onContainerTriggerTriggered(Trigger* t)
{
	if (t == installTriger)
	{
		installModule();
	}
	else if (t == uninstallTrigger)
	{
		AlertWindow::showAsync(
			MessageBoxOptions().withIconType(AlertWindow::QuestionIcon)
			.withTitle("Remove custom module")
			.withMessage("Are you sure you want to remove this custom module ?")
			.withButton("Yes")
			.withButton("No"),
			[this](int result)
			{
				if (result)
				{
					if (localModuleFolder.exists()) localModuleFolder.deleteRecursively();
					ModuleManager::getInstance()->factory->updateCustomModules();

					updateLocalData();
				}
			}
		);
	}
}

void CommunityModuleInfo::finished(URL::DownloadTask* task, bool success)
{
	if (!success)
	{
		LOGERROR("Error while downloading " + url + ",\ntry downloading it directly from the website.\nError code : " + String(task->statusCode()));
		return;
	}

	File moduleFile = getDownloadFilePath();
	File modulesDir = ModuleManager::getInstance()->factory->getCustomModulesFolder();

	if (localModuleFolder.exists()) localModuleFolder.deleteRecursively();

	ZipFile zip(moduleFile);
	zip.uncompressTo(modulesDir);

	ModuleManager::getInstance()->factory->updateCustomModules();
	localModuleFolder = ModuleManager::getInstance()->factory->getFolderForCustomModule(niceName);

	updateLocalData();
}

void CommunityModuleInfo::progress(URL::DownloadTask* /*task*/, int64 /*bytesDownloaded*/, int64 /*totalLength*/)
{
}


InspectableEditor* CommunityModuleInfo::getEditorInternal(bool isRoot)
{
	return new CommunityModuleInfoEditor(this, isRoot);
}


