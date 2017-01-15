/*
  ==============================================================================

    UserOSCCommandModelManagerUI.cpp
    Created: 19 Dec 2016 3:06:08pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommandModelManagerUI.h"

juce_ImplementSingleton(UserOSCCommandModelManagerWindow)


UserOSCCommandModelManagerWindow::UserOSCCommandModelManagerWindow() :
	ResizableWindow("OSC Command Model Manager",false)
{
}

UserOSCCommandModelManagerWindow::~UserOSCCommandModelManagerWindow()
{
}


void UserOSCCommandModelManagerWindow::editModule(CustomOSCModule * _module)
{
	if (_module == module) return;

	
	if (module != nullptr)
	{

	}

	module = _module;

	if (module != nullptr)
	{
		DBG("Edit module : " << _module->niceName);
		addToDesktop();
		setResizable(true, true);
		setDraggable(true);
		setBounds(Rectangle<int>(10, 10, 500, 500));
		setVisible(true);
		toFront(true);
	}
}

void UserOSCCommandModelManagerWindow::userTriedToCloseWindow()
{
	//Close window
	setVisible(false);
	removeFromDesktop();
}

//ManagerUI

UserOSCCommandModelManagerUI::UserOSCCommandModelManagerUI(UserOSCCommandModelManager * manager) :
	BaseManagerUI("Models",manager)
{
}

UserOSCCommandModelManagerUI::~UserOSCCommandModelManagerUI()
{
}

