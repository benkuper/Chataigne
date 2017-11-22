/*
  ==============================================================================

    UserOSCCommandModelManagerUI.cpp
    Created: 19 Dec 2016 3:06:08pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommandModelManagerUI.h"



UserOSCCommandModelManagerWindow::UserOSCCommandModelManagerWindow(const String &_name) :
	ShapeShifterContentComponent(_name)
{
	InspectableSelectionManager::mainSelectionManager->addSelectionListener(this);
	
}

UserOSCCommandModelManagerWindow::~UserOSCCommandModelManagerWindow()
{
	if(InspectableSelectionManager::mainSelectionManager != nullptr) InspectableSelectionManager::mainSelectionManager->removeSelectionListener(this);
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

void UserOSCCommandModelManagerWindow::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::mainSelectionManager->isEmpty()) editModule(nullptr);
	
	Inspectable * i = InspectableSelectionManager::mainSelectionManager->currentInspectables[0];
	CustomOSCModule * m = dynamic_cast<CustomOSCModule *>(i);
	if(m != nullptr) editModule(m);
}

//ManagerUI

UserOSCCommandModelManagerUI::UserOSCCommandModelManagerUI(UserOSCCommandModelManager * manager, const String &moduleName) :
	BaseManagerUI("Command Models for " + moduleName,manager)
{
	drawContour = true;
	addExistingItems();
}

UserOSCCommandModelManagerUI::~UserOSCCommandModelManagerUI()
{
}

