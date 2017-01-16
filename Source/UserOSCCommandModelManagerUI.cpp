/*
  ==============================================================================

    UserOSCCommandModelManagerUI.cpp
    Created: 19 Dec 2016 3:06:08pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommandModelManagerUI.h"

juce_ImplementSingleton(UserOSCCommandModelManagerWindow)


UserOSCCommandModelManagerWindow::UserOSCCommandModelManagerWindow(const String &_name) :
	ShapeShifterContentComponent(_name)
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
		removeChildComponent(modelManagerUI);
		modelManagerUI = nullptr;
	}

	module = _module;

	if (module != nullptr)
	{
		modelManagerUI = new UserOSCCommandModelManagerUI(&module->umm, module->niceName);
		contentComponent->addAndMakeVisible(modelManagerUI);
		resized();
	}
}

void UserOSCCommandModelManagerWindow::resized()
{
	if (modelManagerUI != nullptr) modelManagerUI->setBounds(getLocalBounds().reduced(2));
}

//ManagerUI

UserOSCCommandModelManagerUI::UserOSCCommandModelManagerUI(UserOSCCommandModelManager * manager, const String &moduleName) :
	BaseManagerUI("Models for " + moduleName,manager)
{
	drawContour = true;
}

UserOSCCommandModelManagerUI::~UserOSCCommandModelManagerUI()
{
}

