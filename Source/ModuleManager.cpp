/*
  ==============================================================================

    ModuleManager.cpp
    Created: 8 Dec 2016 2:36:11pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleManager.h"
#include "ModuleFactory.h"

juce_ImplementSingleton(ModuleManager)

ModuleManager::ModuleManager() :
	BaseManager<Module>("Modules")
{

}

ModuleManager::~ModuleManager()
{
	ModuleFactory::deleteInstance();
}

void ModuleManager::addItemFromData(var data)
{
	String moduleType = data.getProperty("type", "none");
	if (moduleType.isEmpty()) return;
	Module * i = ModuleFactory::getInstance()->createModule(moduleType);
	if (i != nullptr) addItem(i, data);
}

PopupMenu ModuleManager::getAllModulesCommandMenu(CommandContext context)
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++) menu.addSubMenu(items[i]->niceName, items[i]->defManager.getCommandMenu(i * 1000,context));
	return menu;
}


CommandDefinition * ModuleManager::getCommandDefinitionForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int outputIndex = (int)floor(itemID / 1000);
	int commandIndex = itemID % 1000 - 1;
	return items[outputIndex]->defManager.definitions[commandIndex];
}
