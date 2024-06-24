/*
  ==============================================================================

    CommunityModuleManager.h
    Created: 29 Jan 2019 3:52:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class CommunityModuleManager :
	public URL::DownloadTask::Listener
{
public:
	juce_DeclareSingleton(CommunityModuleManager, true);

	CommunityModuleManager();
	~CommunityModuleManager();

	std::unique_ptr<URL::DownloadTask> downloadTask;
	String installingModuleName;

	bool installModule(const String& url, const String& moduleName);
	File getDownloadFilePath(const String& moduleName);
	virtual void progress(URL::DownloadTask* task, int64 bytesDownloaded, int64 totalLength) override;
	virtual void finished(URL::DownloadTask* task, bool success) override;
};