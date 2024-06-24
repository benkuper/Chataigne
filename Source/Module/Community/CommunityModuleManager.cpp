/*
  ==============================================================================

	CommunityModuleManager.cpp
	Created: 29 Jan 2019 3:52:46pm
	Author:  bkupe

  ==============================================================================
*/

juce_ImplementSingleton(CommunityModuleManager)

CommunityModuleManager::CommunityModuleManager()
{
}

CommunityModuleManager::~CommunityModuleManager()
{
}


bool CommunityModuleManager::installModule(const String& url, const String& moduleName)
{
	installingModuleName = moduleName;
	File df = getDownloadFilePath(installingModuleName);
	if (df.exists()) df.deleteFile();

	DBG("Download to file " << url << " > " << df.getFullPathName());

	downloadTask = nullptr;
	downloadTask = URL(url).downloadToFile(df, URL::DownloadTaskOptions().withListener(this));

	if (downloadTask == nullptr)
	{
		LOGERROR("Error trying to download module " + moduleName);
		return false;
	}

	return true;
}

File CommunityModuleManager::getDownloadFilePath(const String& moduleName)
{
	File tmpFolder = File::getSpecialLocation(File::tempDirectory).getChildFile("Chataigne");
	if (!tmpFolder.exists()) tmpFolder.createDirectory();
	return tmpFolder.getChildFile(moduleName + ".zip");
}

void CommunityModuleManager::finished(URL::DownloadTask* task, bool success)
{
	if (!success)
	{
		LOGERROR("Error while downloading " + installingModuleName + ",\ntry downloading it directly from the website.\nError code : " + String(task->statusCode()));
		return;
	}

	File moduleFile = getDownloadFilePath(installingModuleName);
	File modulesDir = ModuleManager::getInstance()->factory->getCustomModulesFolder();

	File moduleFolder = modulesDir.getChildFile(installingModuleName);
	if (moduleFolder.exists()) moduleFolder.deleteRecursively();

	ZipFile zip(moduleFile);
	zip.uncompressTo(modulesDir);

	ModuleManager::getInstance()->factory->updateCustomModules();
	moduleFolder = ModuleManager::getInstance()->factory->getFolderForCustomModule(installingModuleName);
}

void CommunityModuleManager::progress(URL::DownloadTask* /*task*/, int64 /*bytesDownloaded*/, int64 /*totalLength*/)
{
	//float percent = (int)(bytesDownloaded * 100.0f / totalLength);
	//LOG("Installing " << installingModuleName << "... " << percent << "%");
}
