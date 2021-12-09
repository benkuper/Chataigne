/*
  ==============================================================================

    AboutWindow.cpp
    Created: 4 Jan 2018 7:25:58pm
    Author:  Ben

  ==============================================================================
*/

AboutWindow::AboutWindow() :
	Component("About")
{
	aboutImage = ChataigneAssetManager::getInstance()->getAboutImage();
	setSize(aboutImage.getWidth(), aboutImage.getHeight());
}

AboutWindow::~AboutWindow()
{
}

void AboutWindow::paint(Graphics &g) 
{
	g.fillAll(BG_COLOR.darker());
	g.drawImage(aboutImage, getLocalBounds().toFloat());
}
