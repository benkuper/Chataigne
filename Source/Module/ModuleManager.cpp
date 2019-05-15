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
#include "CustomVariables/CVGroupManager.h"
#include "Module/modules/customvariables/CustomVariablesModule.h"
#include "ChataigneEngine.h"

juce_ImplementSingleton(ModuleManager)

ModuleManager::ModuleManager() :
	BaseManager<Module>("Modules")
{
	itemDataType = "Module";
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
	if (moduleName == CVGroupManager::getInstance()->module->shortName) return CVGroupManager::getInstance()->module.get();
	if (moduleName == static_cast<ChataigneEngine *>(Engine::mainEngine)->module.shortName) return &static_cast<ChataigneEngine *>(Engine::mainEngine)->module;
	else return getItemWithName(moduleName);
}

void ModuleManager::addItemInternal(Module * module, var data)
{
	module->templateManager.setupTemplateDefinition();
}

Controllable * ModuleManager::showAllValuesAndGetControllable(bool showTriggers, bool showParameters)
{
	PopupMenu menu;
	
	Array<Module *> mList = ModuleManager::getInstance()->getModuleList();
	int numItems = mList.size();

	const int maxValuesPerModule = 10000;
	OwnedArray<ControllableChooserPopupMenu> moduleMenus;

	for (int i = 0; i < numItems; i++)
	{
		Module * m = mList[i];
		ControllableChooserPopupMenu *vCC = new ControllableChooserPopupMenu(&m->valuesCC, showParameters, showTriggers, i* maxValuesPerModule);
		moduleMenus.add(vCC);
		if (i == ModuleManager::getInstance()->items.size()) menu.addSeparator(); // Separator between user created module and special modules
		menu.addSubMenu(m->niceName, *vCC);
	}

	ControllableChooserPopupMenu engineMenu(Engine::mainEngine, showParameters, showTriggers, -100000);
	menu.addSubMenu("Generic", engineMenu);
	
	int result = menu.show();

	if (result < 0)
	{
		return engineMenu.getControllableForResult(result);
	} else
	{
		ControllableChooserPopupMenu * mm = moduleMenus[(int)floorf(result / maxValuesPerModule)];
		return mm->getControllableForResult(result);
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
	for (int i = 0; i < items.size(); i++) menu.addSubMenu(items[i]->niceName, items[i]->getCommandMenu(i * 1000,context));
	menu.addSeparator();
	menu.addSubMenu(StateManager::getInstance()->module.niceName, StateManager::getInstance()->module.getCommandMenu(-1000, context));
	menu.addSubMenu(SequenceManager::getInstance()->module.niceName, SequenceManager::getInstance()->module.getCommandMenu(-2000, context));
	menu.addSubMenu(CVGroupManager::getInstance()->module->niceName, CVGroupManager::getInstance()->module->getCommandMenu(-3000, context));
	menu.addSubMenu(static_cast<ChataigneEngine *>(Engine::mainEngine)->module.niceName, static_cast<ChataigneEngine *>(Engine::mainEngine)->module.getCommandMenu(-10000, context));
	return menu;
}


CommandDefinition * ModuleManager::getCommandDefinitionForItemID(int itemID, Module * lockedModule)
{

	if (itemID == 0) return nullptr;

	Module * m = lockedModule;

	if (itemID < -9000)
	{
		m = &static_cast<ChataigneEngine *>(Engine::mainEngine)->module;
		itemID += 10000;
	} else if (itemID < -2000)
	{
		m = CVGroupManager::getInstance()->module.get();
		itemID += 3000;
	} else if (itemID < -1000)
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
	return m->getCommandDefinitionForItemID(commandIndex);
}

Array<Module*> ModuleManager::getModuleList(bool includeSpecialModules)
{
	Array<Module *> mList;
	for (auto &m : ModuleManager::getInstance()->items) mList.add(m);
	
	if (includeSpecialModules)
	{
		mList.add(&StateManager::getInstance()->module);
		mList.add(&SequenceManager::getInstance()->module);
		mList.add(CVGroupManager::getInstance()->module.get());
	}
	
	return mList;
}
