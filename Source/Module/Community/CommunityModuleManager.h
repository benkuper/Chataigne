/*
  ==============================================================================

    CommunityModuleManager.h
    Created: 29 Jan 2019 3:52:46pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#include "CommunityModuleInfo.h"

class CommunityModuleManager :
	public BaseManager<CommunityModuleInfo>,
	public Thread
{
public:
	juce_DeclareSingleton(CommunityModuleManager, true);

	CommunityModuleManager();
	~CommunityModuleManager();

	void run() override;

	var getJSONDataForURL(URL url);

	CommunityModuleInfo* getModuleInfoForFolder(const File & folder);

	static bool openStreamProgressCallback(void* context, int /*bytesSent*/, int /*totalBytes*/);
};