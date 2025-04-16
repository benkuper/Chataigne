#include "ModuleManagerUI.h"
/*
  ==============================================================================

    ModuleManagerUI.cpp
    Created: 8 Dec 2016 2:36:57pm
    Author:  Ben

  ==============================================================================
*/

ModuleManagerUI::ModuleManagerUI(const String &contentName, ModuleManager * _manager) :
	ManagerShapeShifterUI<ModuleManager, Module, ModuleUI>(contentName, _manager)
{
	noItemText = "Start here by adding a module to connect to another software or a physical device.";

	setShowSearchBar(true);
	addExistingItems();
}

ModuleManagerUI::~ModuleManagerUI()
{
}

ModuleUI* ModuleManagerUI::createUIForItem(Module* item)
{
	return item->getModuleUI();
}

void ModuleManagerUI::addItemFromMenu(Module* m, bool fromAddButton, Point<int> pos)
{
	ManagerShapeShifterUI::addItemFromMenu(m, fromAddButton, pos);
	m->setupFromManualCreation();
}
