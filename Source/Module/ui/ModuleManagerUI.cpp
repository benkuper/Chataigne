/*
  ==============================================================================

	ModuleManagerUI.cpp
	Created: 8 Dec 2016 2:36:57pm
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

ModuleManagerUI::ModuleManagerUI(const String& contentName, ModuleManager* _manager) :
	ManagerUI<ModuleManager, Module, ModuleUI, ItemBaseGroup<Module>, ModuleGroupUI>(contentName, _manager, _manager == RootModuleManager::getInstance())
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
	ManagerUI::addItemFromMenu(m, fromAddButton, pos);
	m->setupFromManualCreation();
}



RootModuleManagerUI::RootModuleManagerUI(const String& contentName, ModuleManager* _manager) :
	ModuleManagerUI(contentName, _manager),
	ShapeShifterContent(this, contentName)
{

}

RootModuleManagerUI::~RootModuleManagerUI()
{
}
