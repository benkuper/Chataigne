/*
  ==============================================================================

    ModuleManager.cpp
    Created: 8 Dec 2016 2:36:11pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleManager.h"
#include "ModuleFactory.h"
#include "StateManager.h"
#include "SequenceManager.h"
#include "ChataigneEngine.h"

juce_ImplementSingleton(ModuleManager)

ModuleManager::ModuleManager() :
	BaseManager<Module>("Modules")
{

}

ModuleManager::~ModuleManager()
{
	ModuleFactory::deleteInstance();
}

void ModuleManager::addItemFromData(var data, bool fromUndoableAction)
{
	String moduleType = data.getProperty("type", "none");
	if (moduleType.isEmpty()) return;
	Module * i = ModuleFactory::getInstance()->createModule(moduleType);
	if (i != nullptr) addItem(i, data, fromUndoableAction);
}

Module * ModuleManager::getModuleWithName(const String & moduleName)
{
	//DBG("get Module with name : " << moduleName << " / " << StateManager::getInstance()->shortName);
	if (moduleName == StateManager::getInstance()->module.shortName) return &StateManager::getInstance()->module;
	if (moduleName == SequenceManager::getInstance()->module.shortName) return &SequenceManager::getInstance()->module;
	if (moduleName == static_cast<ChataigneEngine *>(Engine::mainEngine)->module.shortName) return &static_cast<ChataigneEngine *>(Engine::mainEngine)->module;
	else return getItemWithName(moduleName);
}

PopupMenu ModuleManager::getAllModulesInputValuesMenu(bool parametersOnly)
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++)
	{
		PopupMenu sMenu;
		int numValues = items[i]->valuesCC.controllables.size();
		for (int j = 0; j < numValues; j++)
		{
			Controllable * c = items[i]->valuesCC.controllables[j];
			if (c->type == Controllable::TRIGGER && parametersOnly) continue;
			//DBG("Add menu item " << c->niceName << " / " << i*1000+j+1);
			sMenu.addItem(i * 1000 + j + 1, c->niceName);
		}
		menu.addSubMenu(items[i]->niceName, sMenu);
	}

	return menu;
}

Controllable * ModuleManager::getControllableForItemID(int itemID)
{
	if (itemID <= 0) return nullptr;
	int moduleIndex = (int)floor((itemID-1) / 1000);
	int valueIndex = (itemID-1) % 1000;
	//DBG("Get Controllable for ItemID : " << itemID << " / " <<moduleIndex << " / "<<valueIndex);
	return items[moduleIndex]->valuesCC.controllables[valueIndex];
}

PopupMenu ModuleManager::getAllModulesCommandMenu(CommandContext context)
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); i++) menu.addSubMenu(items[i]->niceName, items[i]->defManager.getCommandMenu(i * 1000,context));
	menu.addSubMenu(StateManager::getInstance()->module.niceName, StateManager::getInstance()->module.defManager.getCommandMenu(-1000, context));
	menu.addSubMenu(SequenceManager::getInstance()->module.niceName, SequenceManager::getInstance()->module.defManager.getCommandMenu(-2000, context));
	menu.addSubMenu(static_cast<ChataigneEngine *>(Engine::mainEngine)->module.niceName, static_cast<ChataigneEngine *>(Engine::mainEngine)->module.defManager.getCommandMenu(-3000, context));
	return menu;
}


CommandDefinition * ModuleManager::getCommandDefinitionForItemID(int itemID, Module * lockedModule)
{
	if (itemID == 0) return nullptr;
	DBG("Item id = " + String(itemID));

	Module * m = lockedModule;

	if (itemID < -2000)
	{
		m = &static_cast<ChataigneEngine *>(Engine::mainEngine)->module;
		itemID += 3000;
	}else if (itemID < -1000)
	{
		m = &SequenceManager::getInstance()->module;
		itemID += 2000;
	}else if (itemID < 0)
	{
		m = &StateManager::getInstance()->module;
		itemID += 1000;
	}else if (m == nullptr)
	{
		int moduleIndex = (int)floor(itemID / 1000);
		m = items[moduleIndex];
	}

	DBG("M not null ?" << (int)(m != nullptr));
	
	if (m == nullptr) return nullptr;

	int commandIndex = itemID % 1000 - 1;
	return m->defManager.definitions[commandIndex];
}
