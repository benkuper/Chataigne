/*
  ==============================================================================

	CommunityModuleInfo.cpp
	Created: 29 Jan 2019 3:52:31pm
	Author:  bkupe

  ==============================================================================
*/

#include "CommunityModuleInfo.h"
#include "ui/CommunityModuleInfoEditor.h"
#include "Module/ModuleFactory.h"

CommunityModuleInfo::CommunityModuleInfo(StringRef name, var onlineData, var localData, File localFolder) :
	BaseItem(name, false)
{
	userCanRemove = false;
	canBeReorderedInEditor = false;

	installTriger = addTrigger("Install", "Install or Update this module");
	uninstallTrigger = addTrigger("Uninstall", "Uninstall this module");	
	isOnline = addBoolParameter("Is Online", "Is this module registered online ?", !onlineData.isVoid());
	isLocal = addBoolParameter("Is Local", "Is this module installed locally ?", localFolder.exists());

	DBG("New Module Info " << JSON::toString(onlineData) << " / " << localFolder.getFullPathName());

	if (!onlineData.isVoid())
	{
		url = onlineData.getProperty("url", "");
		description = onlineData.getProperty("description", "");
		downloadURL = onlineData.getProperty("downloadURL", "");
		onlineVersion = onlineData.getProperty("version", "0.0.0");
	}

	if (!localData.isVoid())
	{
		localVersion = localData.getProperty("version", "0.0.0");
		if (localFolder.exists()) localModuleFolder = localFolder;
	}
}

CommunityModuleInfo::~CommunityModuleInfo()
{
}

void CommunityModuleInfo::installModule()
{
	DBG("Download to file " << downloadURL << " > " << getDownloadFilePath().getFullPathName());
	downloadTask = URL(downloadURL).downloadToFile(getDownloadFilePath(), "", this);
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

void CommunityModuleInfo::onContainerTriggerTriggered(Trigger * t)
{
	if (t == installTriger)
	{
		installModule();
	}
	else if(t == uninstallTrigger)
	{
		int result = AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon, "Remove custom module", "Are you sure you want to remove this custom module ?", "Yes", "No");
		if (result)
		{
			DBG("Local folder exists ? " << localModuleFolder.getFullPathName() << " : " << (int)localModuleFolder.exists());
			if (localModuleFolder.exists()) localModuleFolder.deleteRecursively();
			ModuleFactory::getInstance()->updateCustomModules();
			isLocal->setValue(false);
		}
	}
}

void CommunityModuleInfo::finished(URL::DownloadTask * task, bool success)
{
	if (!success)
	{
		LOGERROR("Error while downloading " + url + ",\ntry downloading it directly from the website.\nError code : " + String(task->statusCode()));
		return;
	}

	File moduleFile = getDownloadFilePath();
	File modulesDir = ModuleFactory::getInstance()->getCustomModulesFolder();

	if (localModuleFolder.exists()) localModuleFolder.deleteRecursively();

	ZipFile zip(moduleFile);
	zip.uncompressTo(modulesDir);

	ModuleFactory::getInstance()->updateCustomModules();
	localModuleFolder = ModuleFactory::getInstance()->getFolderForCustomModule(niceName);
	isLocal->setValue(localModuleFolder.exists());
}

void CommunityModuleInfo::progress(URL::DownloadTask * task, int64 bytesDownloaded, int64 totalLength)
{
}


InspectableEditor * CommunityModuleInfo::getEditor(bool isRoot)
{
	return new CommunityModuleInfoEditor(this, isRoot);
}


