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
	public Thread,
	public Timer
{
public:
	juce_DeclareSingleton(MatomoAnalytics, true)

	MatomoAnalytics();
	~MatomoAnalytics();

	enum AnalyticsAction { NONE, START, PING, STOP, CRASH, MAX_ACTIONS };
	const String actionNames[MAX_ACTIONS] {"None", "App / Start", "Ping", "App / Stop", "App / Crash"};

	URL baseURL;

	String actionToLog;
	StringPairArray optionsToLog;
	bool actionIsPing;

	bool hasAlreadyShownWarning;
	
	void log(AnalyticsAction action, StringPairArray options = {});
	void log(StringRef actionName, StringPairArray options = {});

	void run() override;

	void timerCallback() override;
};