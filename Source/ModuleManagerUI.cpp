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
	useDefaultMenu = false;

}

ModuleManagerUI::~ModuleManagerUI()
{
}

void ModuleManagerUI::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if (e.mods.isRightButtonDown())
	{
		Module * i = ModuleFactory::getInstance()->showCreateMenu();
		if (i != nullptr) manager->addItem(i);
	}
}