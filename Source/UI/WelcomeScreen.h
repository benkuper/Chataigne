/*
  ==============================================================================

    WelcomeScreen.h
    Created: 25 Oct 2019 11:14:49am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"

class WelcomeScreen :
	public Component
{
public:
	WelcomeScreen();
	~WelcomeScreen();

	void paint(Graphics& g) override;
	void resized() override;

	class WebComp :
		public WebBrowserComponent
	{
	public:
		WebComp() {}
		~WebComp() {}

		bool pageAboutToLoad(const String& url) override { DBG("About to load : " << url); return true;  }
		void pageFinishedLoading(const String& url) override { DBG("Finished loading : " << url); }
		bool pageLoadHadNetworkError(const String& error) override { DBG("Error loading welcome : " << error); return true; }
	};

	WebComp webComp;

};

