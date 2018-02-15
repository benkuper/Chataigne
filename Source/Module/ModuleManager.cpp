/*
  ==============================================================================

    ModuleManager.cpp
    Created: 8 Dec 2016 2:36:11pm
    Author:  Ben

  ==============================================================================
*/

#include "ModuleManager.h"
#include "ModuleFactory.h"
#include "StateMachine/StateManager.h"
#include "TimeMachine/SequenceManager.h"
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
	Array<WeakReference<Controllable>> controllableMenuRefs;
	int curID = 1;

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

			sMenu.addItem(curID, c->niceName);
			controllableMenuRefs.add(c);
			curID++;
		}

		int numContainers = m->valuesCC.controllableContainers.size();
		for (int k = 0; k < numContainers; k++)
		{
			ControllableContainer * vcc = m->valuesCC.controllableContainers[k];
			PopupMenu cMenu;
			int numCValues = vcc->controllables.size();
			for (int kj = 0; kj < numCValues; kj++)
			{
				Controllable * c = vcc->controllables[kj];
				if (c->type == Controllable::TRIGGER) { if (!showTriggers) continue; } else if (!showParameters) continue;
				cMenu.addItem(curID, c->niceName);
				controllableMenuRefs.add(c);
				curID++;
			}
			sMenu.addSubMenu(vcc->niceName, cMenu);
		}

		menu.addSubMenu(m->niceName, sMenu);
	}

	//TODO : move from here the handling of other values than modules
	ControllableChooserPopupMenu engineMenu(Engine::mainEngine, showParameters,showTriggers, numItems * 10000);
	menu.addSubMenu("Generic", engineMenu);
	int itemID = menu.show();

	if (itemID >= 10000)
	{
		return engineMenu.getControllableForResult(itemID);
	} else
	{
		return controllableMenuRefs[itemID-1];
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
