/*
  ==============================================================================

    WelcomeScreen.h
    Created: 25 Oct 2019 11:14:49am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class WelcomeScreen :
	public Component
{
public:
	WelcomeScreen();
	~WelcomeScreen();

	void paint(Graphics& g) override;
	void resized() override;

	class WebComp :
#if JUCE_WINDOWS
		public WindowsWebView2WebBrowserComponent
#else
		public WebBrowserComponenet
#endif
	{
	public:
		WebComp()
#if JUCE_WINDOWS
			: WindowsWebView2WebBrowserComponent(true,WebView2Preferences())
#endif
		{}
		~WebComp() {}
		String baseURL;
		void setBaseURL(StringRef url);
		bool pageAboutToLoad(const String& url) override;
		void pageFinishedLoading(const String& url) override;
		bool pageLoadHadNetworkError(const String& error) override;
	};

	WebComp webComp;

};

