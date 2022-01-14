/*
  ==============================================================================

	CommunityModuleInfo.h
	Created: 29 Jan 2019 3:52:31pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

class CommunityModuleInfo :
	public BaseItem,
	public URL::DownloadTask::Listener
{
public:
	CommunityModuleInfo(StringRef name = "", var moduleData = var());
	~CommunityModuleInfo();

	enum CommunityModuleStatus {INSTALLED, NEW_VERSION_AVAILABLE, UP_TO_DATE, NOT_INSTALLED};

	String url;
	String description;
	String downloadURL;
	String onlineVersion;
	String localVersion;
	File localModuleFolder;
	CommunityModuleStatus status;

	std::unique_ptr<URL::DownloadTask> downloadTask;

	BoolParameter* isLocal;
	BoolParameter* isOnline;
	Trigger* installTriger;
	Trigger* uninstallTrigger;

	void installModule();
	File getDownloadFilePath();

	void updateLocalData();

	void onContainerTriggerTriggered(Trigger* t) override;

	// Inherited via Listener
	virtual void progress(URL::DownloadTask* task, int64 bytesDownloaded, int64 totalLength) override;
	virtual void finished(URL::DownloadTask* task, bool success) override;


	InspectableEditor* getEditorInternal(bool isRoot) override;
};