/*
  ==============================================================================

    ModuleManagerUI.cpp
    Created: 8 Dec 2016 2:36:57pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleManagerUI.h"
#include "ModuleFactory.h"

ModuleManagerUI::ModuleManagerUI(const String &contentName, ModuleManager * _manager) :
	BaseManagerShapeShifterUI<ModuleManager, Module, ModuleUI>(contentName, _manager)
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