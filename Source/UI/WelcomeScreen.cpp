/*
  ==============================================================================

    WelcomeScreen.cpp
    Created: 25 Oct 2019 11:14:49am
    Author:  bkupe

  ==============================================================================
*/

#include "WelcomeScreen.h"

WelcomeScreen::WelcomeScreen()
{
	setSize(820,620);
	addAndMakeVisible(webComp);

	webComp.goToURL("http://benjamin.kuperberg.fr/chataigne/welcome/welcome.html", nullptr, nullptr);
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
	webComp.setBounds(getLocalBounds().reduced(10));
}
