/*
  ==============================================================================

    Main.h
    Created: 25 Oct 2016 11:16:59pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once
#pragma warning(disable:4244 4100 4305 26451 26495)

#include "MainComponent.h"
#include "ChataigneEngine.h"
#include "Analytics/GoogleAnalyticsDestination.h"

//==============================================================================
class ChataigneApplication : public OrganicApplication
{
public:
	//==============================================================================
	ChataigneApplication();

	//---- GLOBAL SETTINGS CHATAIGNE SPECIFIC
	BoolParameter * enableSendAnalytics;
	//

	void initialiseInternal(const String& /*commandLine*/) override;
	void afterInit() override;

	void shutdown() override;
};


START_JUCE_APPLICATION(ChataigneApplication)