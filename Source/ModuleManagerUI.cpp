/*
  ==============================================================================

    ModuleManagerUI.cpp
    Created: 8 Dec 2016 2:36:57pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleManagerUI.h"
#include "ModuleFactory.h"

ModuleManagerUI::ModuleManagerUI(ModuleManager * _manager) :
	BaseManagerShapeShifterUI<ModuleManager, Module, ModuleUI>("Modules", _manager)
{
	addExistingItems();
}

ModuleManagerUI::~ModuleManagerUI()
{
}

void ModuleManagerUI::showMenuAndAddItem(bool, Point<int>)
{
	Module * m = ModuleFactory::getInstance()->showCreateMenu();
	if (m != nullptr) manager->addItem(m);
}