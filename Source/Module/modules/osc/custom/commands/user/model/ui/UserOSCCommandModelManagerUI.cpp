/*
  ==============================================================================

    UserOSCCommandModelManagerUI.cpp
    Created: 19 Dec 2016 3:06:08pm
    Author:  Ben

  ==============================================================================
*/

#include "UserOSCCommandModelManagerUI.h"



UserOSCCommandModelManagerWindow::UserOSCCommandModelManagerWindow(const String &_name) :
	ShapeShifterContentComponent(_name),
	module(nullptr)
{

	InspectableSelectionManager::mainSelectionManager->addSelectionListener(this);
	helpID = "OSCCommandModel";


	if (!InspectableSelectionManager::mainSelectionManager->isEmpty())
	{
		Inspectable * i = InspectableSelectionManager::mainSelectionManager->currentInspectables[0];
		Module * m = dynamic_cast<Module *>(i);
		if (m != nullptr)
		{
			CustomOSCModule * om = dynamic_cast<CustomOSCModule *>(i);
			editModule(om);
		}
	}
}

UserOSCCommandModelManagerWindow::~UserOSCCommandModelManagerWindow()
{
	if(InspectableSelectionManager::mainSelectionManager != nullptr) InspectableSelectionManager::mainSelectionManager->removeSelectionListener(this);
	editModule(nullptr);
}


void UserOSCCommandModelManagerWindow::editModule(CustomOSCModule * _module)
{
	if (_module == module) return;

	
	if (module != nullptr)
	{
		removeChildComponent(modelManagerUI);
		module->removeInspectableListener(this);
		modelManagerUI = nullptr;
	}

	module = _module;

	if (module != nullptr)
	{
		modelManagerUI = new UserOSCCommandModelManagerUI(&module->umm, module->niceName);
		contentComponent->addAndMakeVisible(modelManagerUI);
		module->addInspectableListener(this);
		resized();
	}
}

void UserOSCCommandModelManagerWindow::resized()
{
	ShapeShifterContentComponent::resized();
	if (modelManagerUI != nullptr) modelManagerUI->setBounds(getLocalBounds().reduced(2));
}

void UserOSCCommandModelManagerWindow::paint(Graphics & g)
{
	//ShapeShifterContentComponent::paint(g);
	
	if (module == nullptr)
	{
		g.setColour(Colours::white.withAlpha(.4f));
		g.setFont(16);
		g.drawFittedText("Select a Generic OSC Module to edit its command models", getLocalBounds().reduced(20), Justification::centred, 6);
	}
}

void UserOSCCommandModelManagerWindow::inspectablesSelectionChanged()
{
	if (InspectableSelectionManager::mainSelectionManager->isEmpty()) return;
	Inspectable * i = InspectableSelectionManager::mainSelectionManager->currentInspectables[0];
	Module * m = dynamic_cast<Module *>(i);
	if (m != nullptr)
	{
		CustomOSCModule * om = dynamic_cast<CustomOSCModule *>(i);
		editModule(om);
	}
}

void UserOSCCommandModelManagerWindow::inspectableDestroyed(Inspectable * i)
{
	if (module == i) editModule(nullptr);
}

//ManagerUI

UserOSCCommandModelManagerUI::UserOSCCommandModelManagerUI(UserOSCCommandModelManager * manager, const String &moduleName) :
	BaseManagerUI("Command Models for " + moduleName,manager)
{
	drawContour = true;

	noItemText = "Here you can add your own commands for the module " + moduleName;

	addExistingItems();
	resized();
	repaint();
}

UserOSCCommandModelManagerUI::~UserOSCCommandModelManagerUI()
{
}

