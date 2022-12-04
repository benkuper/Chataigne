/*
  ==============================================================================

	WelcomeScreen.cpp
	Created: 25 Oct 2019 11:14:49am
	Author:  bkupe

  ==============================================================================
*/

#include "MainIncludes.h"

String getAppVersion();
ApplicationProperties& getAppProperties();

WelcomeScreen::WelcomeScreen()
{
	String lastVersion = getAppProperties().getUserSettings()->getValue("lastVersion", "0");
	bool firstRun = lastVersion.isEmpty();

	setSize(810, 610);
	addAndMakeVisible(webComp);
	webComp.setBaseURL("http://benjamin.kuperberg.fr/chataigne/welcome/welcome.php?firstRun=" + String((int)firstRun) + "&version=" + getAppVersion());
}

WelcomeScreen::~WelcomeScreen()
{
}

void WelcomeScreen::paint(Graphics& g)
{
	g.fillAll(BG_COLOR.brighter(.2f));
}

void WelcomeScreen::resized()
{
	webComp.setBounds(getLocalBounds().reduced(5));
}


WelcomeScreen::WebComp::WebComp() 
#if JUCE_WINDOWS 
	: WebBrowserComponent(WebBrowserComponent::Options().withBackend(WebBrowserComponent::Options::Backend::webview2).withWinWebView2Options(
		WebBrowserComponent::Options::WinWebView2().withUserDataFolder(File::getSpecialLocation(File::windowsLocalAppData).getChildFile("Chataigne")).withStatusBarDisabled()))
#endif
{

}

void WelcomeScreen::WebComp::setBaseURL(StringRef url)
{
	baseURL = url;
	goToURL(baseURL);
}

bool WelcomeScreen::WebComp::pageAboutToLoad(const String& url) {
	DBG("About to load : " << url);
	if (url == baseURL || url == "about:blank") return true;

	URL(url).launchInDefaultBrowser();
	return false;
}

void WelcomeScreen::WebComp::pageFinishedLoading(const String& url) {
	DBG("Finished loading : " << url);
}

bool WelcomeScreen::WebComp::pageLoadHadNetworkError(const String& error) {
	DBG("Error loading welcome : " << error);
	return true;
}
