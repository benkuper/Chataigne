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
		String baseURL;
		void setBaseURL(StringRef url);
		bool pageAboutToLoad(const String& url) override;
		void pageFinishedLoading(const String& url) override;
		bool pageLoadHadNetworkError(const String& error) override;
	};

	WebComp webComp;

};

