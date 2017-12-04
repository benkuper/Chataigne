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
	helpID = "OSCCommandModel";
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

void UserOSCCommandModelManagerWindow::paint(Graphics & g)
{
	ShapeShifterContentComponent::paint(g);
	
	if (module == nullptr)
	{
		g.setColour(Colours::white.withAlpha(.4f));
		g.setFont(16);
		g.drawFittedText("Select a Generic OSC Module to edit its command models", getLocalBounds(), Justification::centred, 6);
	}
}

void UserOSCCommandModelManagerWindow::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::mainSelectionManager->isEmpty()) editModule(nullptr);
	
	Inspectable * i = InspectableSelectionManager::mainSelectionManager->currentInspectables[0];
	Module * m = dynamic_cast<Module *>(i);
	if (m != nullptr)
	{
		CustomOSCModule * om = dynamic_cast<CustomOSCModule *>(i);
		editModule(om);
	}
}

//ManagerUI

UserOSCCommandModelManagerUI::UserOSCCommandModelManagerUI(UserOSCCommandModelManager * manager, const String &moduleName) :
	BaseManagerUI("Command Models for " + moduleName,manager)
{
	drawContour = true;

	noItemText = "Here you can add your own commands for the module " + moduleName;

	addExistingItems();
}

UserOSCCommandModelManagerUI::~UserOSCCommandModelManagerUI()
{
}

