/*
  ==============================================================================

    LiveOSCModule.h
    Created: 13 Apr 2017 11:02:29am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "../OSCModule.h"

class LiveOSCModule :
	public OSCModule,
	public URL::DownloadTask::Listener
{
public:
	LiveOSCModule();
	~LiveOSCModule() {}


	File liveFolder;
	File pluginFolder;
	File downloadedFile;
	std::unique_ptr<URL::DownloadTask> downloadTask;

	void detectAndInstallLivePlugin();

	void progress(URL::DownloadTask * , int64 bytesDownloaded, int64 totalBytes) override;
	void finished(URL::DownloadTask * , bool success) override;

	static LiveOSCModule * create() { return new LiveOSCModule(); }
	virtual String getDefaultTypeString() const override { return "LiveOSC2"; }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LiveOSCModule)
};