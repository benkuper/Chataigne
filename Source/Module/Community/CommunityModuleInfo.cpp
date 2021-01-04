/*
  ==============================================================================

	CommunityModuleInfo.cpp
	Created: 29 Jan 2019 3:52:31pm
	Author:  bkupe

  ==============================================================================
*/

#include "CommunityModuleInfo.h"
#include "ui/CommunityModuleInfoEditor.h"
#include "Module/ModuleManager.h"
#include "Module/ModuleFactory.h"

CommunityModuleInfo::CommunityModuleInfo(StringRef name, var onlineData) :
	BaseItem(name, false)
{
	nameCanBeChangedByUser = false;
	userCanRemove = false;
	canBeReorderedInEditor = false;
	editorIsCollapsed = true;

	installTriger = addTrigger("Install", "Install or Update this module");
	uninstallTrigger = addTrigger("Uninstall", "Uninstall this module");	

	isOnline = addBoolParameter("Is Online", "Is this module registered online ?", !onlineData.isVoid());
	isLocal = addBoolParameter("Is Local", "Is this module installed locally ?",  false);
	
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
	downloadTask = URL(downloadURL).downloadToFile(df, "", this);
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

	installTriger->setEnabled(onlineVersion != localVersion);

	isLocal->setValue(!localData.isVoid());
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
			if (localModuleFolder.exists()) localModuleFolder.deleteRecursively();
			ModuleManager::getInstance()->factory->updateCustomModules();

			updateLocalData();
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
	File modulesDir = ModuleManager::getInstance()->factory->getCustomModulesFolder();

	if (localModuleFolder.exists()) localModuleFolder.deleteRecursively();

	ZipFile zip(moduleFile);
	zip.uncompressTo(modulesDir);

	ModuleManager::getInstance()->factory->updateCustomModules();
	localModuleFolder = ModuleManager::getInstance()->factory->getFolderForCustomModule(niceName);
	
	updateLocalData();
}

void CommunityModuleInfo::progress(URL::DownloadTask * /*task*/, int64 /*bytesDownloaded*/, int64 /*totalLength*/)
{
}


InspectableEditor * CommunityModuleInfo::getEditor(bool isRoot)
{
	return new CommunityModuleInfoEditor(this, isRoot);
}


