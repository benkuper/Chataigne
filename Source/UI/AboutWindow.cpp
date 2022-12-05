/*
  ==============================================================================

	AboutWindow.cpp
	Created: 4 Jan 2018 7:25:58pm
	Author:  Ben

  ==============================================================================
*/

#include "MainIncludes.h"

AboutWindow::AboutWindow() :
	Component("About")
{
	aboutImage = ChataigneAssetManager::getInstance()->getAboutImage();
	setSize(aboutImage.getWidth(), aboutImage.getHeight());
}

AboutWindow::~AboutWindow()
{
}

void AboutWindow::paint(Graphics& g)
{
	g.fillAll(BG_COLOR.darker());
	g.drawImage(aboutImage, getLocalBounds().toFloat());

	g.setColour(TEXT_COLOR);
	g.setFont(12);
	g.drawText(getApp().getApplicationName() + " " + getApp().getApplicationVersion(), getLocalBounds().removeFromBottom(30).removeFromRight(200).reduced(5).toFloat(), Justification::right);
}
