/*
  ==============================================================================

    MatamoAnalytics.h
    Created: 29 May 2019 2:43:04am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class MatamoAnalytics :
	public Thread
{
public:
	juce_DeclareSingleton(MatamoAnalytics, true)

	MatamoAnalytics();
	~MatamoAnalytics();

	enum AnalyticsAction { NONE, START, STOP, UPDATE, MAX_ACTIONS };
	const String actionNames[MAX_ACTIONS] {"None", "App / Start", "App / Stop", "App / Update"};

	URL baseURL;

	String actionToLog;
	bool actionIsPing;
	
	void log(AnalyticsAction action);
	void log(StringRef actionName);

	void run() override;
};