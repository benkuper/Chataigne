/*
  ==============================================================================

    MatomoAnalytics.h
    Created: 29 May 2019 2:43:04am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class MatomoAnalytics :
	public Thread
{
public:
	juce_DeclareSingleton(MatomoAnalytics, true)

	MatomoAnalytics();
	~MatomoAnalytics();

	enum AnalyticsAction { NONE, START, STOP, CRASH, MAX_ACTIONS };
	const String actionNames[MAX_ACTIONS] {"None", "App / Start", "App / Stop", "App / Crash"};

	URL baseURL;

	String actionToLog;
	StringPairArray optionsToLog;
	bool actionIsPing;
	
	void log(AnalyticsAction action, StringPairArray options = {});
	void log(StringRef actionName, StringPairArray options = {});

	void run() override;
};