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
	helpID = "Modules";
}

ModuleManager::~ModuleManager()
{
	ModuleFactory::deleteInstance();
}

Module * ModuleManager::addItemFromData(var data, bool fromUndoableAction)
{
	String moduleType = data.getProperty("type", "none");
	if (moduleType.isEmpty()) return nullptr;
	Module * i = ModuleFactory::getInstance()->createModule(moduleType);
	if (i != nullptr) return addItem(i, data, fromUndoableAction);
	
	return nullptr;
}

Module * ModuleManager::getModuleWithName(const String & moduleName)
{
	//DBG("get Module with name : " << moduleName << " / " << StateManager::getInstance()->shortName);
	if (moduleName == StateManager::getInstance()->module.shortName) return &StateManager::getInstance()->module;
	if (moduleName == SequenceManager::getInstance()->module.shortName) return &SequenceManager::getInstance()->module;
	if (moduleName == static_cast<ChataigneEngine *>(Engine::mainEngine)->module.shortName) return &static_cast<ChataigneEngine *>(Engine::mainEngine)->module;
	else return getItemWithName(moduleName);
}

Controllable * ModuleManager::showAllValuesAndGetControllable(bool showTriggers, bool showParameters)
{
	PopupMenu menu;
	int numItems = ModuleManager::getInstance()->items.size();
	for (int i = 0; i < numItems; i++)
	{
		Module * m = ModuleManager::getInstance()->items[i];
		PopupMenu sMenu;
		int numValues =  m->valuesCC.controllables.size();
		for (int j = 0; j < numValues; j++)
		{
			Controllable * c = m->valuesCC.controllables[j];
            if (c->type == Controllable::TRIGGER) { if(!showTriggers) continue; }
			else if (!showParameters) continue;

			sMenu.addItem(i * 1000 + j + 1, c->niceName);
		}
		menu.addSubMenu(m->niceName, sMenu);
	}

	//TODO : move from here the handling of other values than modules
	ControllableChooserPopupMenu engineMenu(Engine::mainEngine, showParameters,showTriggers, numItems * 1000);
	menu.addSubMenu("Generic", engineMenu);
	int itemID = menu.show();


	if (itemID <= 0) return nullptr;

	int moduleIndex = (int)floor((itemID-1) / 1000);
	int valueIndex = (itemID-1) % 1000;

	if (moduleIndex >= numItems)
	{
		return engineMenu.getControllableForResult(itemID);
	} else
	{
		return ModuleManager::getInstance()->items[moduleIndex]->valuesCC.controllables[valueIndex];
	}

}

bool ModuleManager::checkControllableIsAValue(Controllable * c)
{
	ControllableContainer * cc = c->parentContainer;
	while (cc != nullptr)
	{
		Module * m = dynamic_cast<Module *>(cc); //If controllable is child of a module
		
		if (c->parentContainer == m || c->parentContainer == &m->moduleParams) return false; //If controllable is direct child of this module, or child 

		if (m != nullptr)
		{
			ControllableContainer * vcc = c->parentContainer;
			while (vcc != nullptr)
			{
				if (vcc == &m->valuesCC) return true; //If controllable is child of this module's valuesCC
				vcc = vcc->parentContainer;
			}

			return false;
		}

		cc = cc->parentContainer;
	}
	return false;
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

	
	if (m == nullptr) return nullptr;

	int commandIndex = itemID % 1000 - 1;
	return m->defManager.definitions[commandIndex];
}
