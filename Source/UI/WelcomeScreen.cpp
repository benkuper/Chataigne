/*
  ==============================================================================

    WelcomeScreen.cpp
    Created: 25 Oct 2019 11:14:49am
    Author:  bkupe

  ==============================================================================
*/

#include "WelcomeScreen.h"

String getAppVersion();
ApplicationProperties& getAppProperties();

WelcomeScreen::WelcomeScreen()
{

	String lastVersion = getAppProperties().getUserSettings()->getValue("lastVersion", "0");
	bool firstRun = lastVersion.isEmpty();
	
	setSize(810,610);
	addAndMakeVisible(webComp);
	webComp.goToURL("http://benjamin.kuperberg.fr/chataigne/welcome/welcome.php?firstRun="+String((int)firstRun)+"&version="+getAppVersion(), nullptr, nullptr);
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

bool WelcomeScreen::WebComp::pageAboutToLoad(const String& url) { 
	DBG("About to load : " << url);  
	return true; 
}

void WelcomeScreen::WebComp::pageFinishedLoading(const String& url) {
	DBG("Finished loading : " << url);
}

bool WelcomeScreen::WebComp::pageLoadHadNetworkError(const String& error) { 
	DBG("Error loading welcome : " << error);
	return true; 
}
