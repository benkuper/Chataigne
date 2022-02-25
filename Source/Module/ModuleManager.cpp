/*
  ==============================================================================

    ModuleManager.cpp
    Created: 8 Dec 2016 2:36:11pm
    Author:  Ben

  ==============================================================================
*/

#include "StateMachine/StateMachineIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include "CustomVariables/CustomVariablesIncludes.h"
#include "MainIncludes.h"

juce_ImplementSingleton(ModuleManager)

ModuleManager::ModuleManager() :
	BaseManager<Module>("Modules")
{
	itemDataType = "Module";
	helpID = "Modules";
	showInspectorOnSelect = false;

	factory.reset(new ModuleFactory());
	managerFactory = factory.get();

	ControllableFactory::getInstance()->controllableDefs.add(new ControllableDefinition(MIDIValueParameter::getTypeStringStatic(), &MIDIValueParameter::create, true));
	ControllableFactory::getInstance()->controllableDefs.add(new ControllableDefinition(DMXValueParameter::getTypeStringStatic(), &DMXValueParameter::create, true));
}

ModuleManager::~ModuleManager()
{
}

Module * ModuleManager::getModuleWithName(const String & moduleName)
{
	//DBG("get Module with name : " << moduleName << " / " << StateManager::getInstance()->shortName);
	if (moduleName == StateManager::getInstance()->module->shortName) return StateManager::getInstance()->module.get();
	if (moduleName == ChataigneSequenceManager::getInstance()->module->shortName) return ChataigneSequenceManager::getInstance()->module.get();
	if (moduleName == CVGroupManager::getInstance()->module->shortName) return CVGroupManager::getInstance()->module.get();
	if (moduleName == static_cast<ChataigneEngine*>(Engine::mainEngine)->module->shortName) return static_cast<ChataigneEngine*>(Engine::mainEngine)->module.get();
	if (moduleName == static_cast<ChataigneEngine *>(Engine::mainEngine)->multiplexModule->shortName) return static_cast<ChataigneEngine *>(Engine::mainEngine)->multiplexModule.get();
	else return getItemWithName(moduleName, true);
}

void ModuleManager::addItemInternal(Module * module, var data)
{
	module->templateManager->setupDefinitionsFromModule();
}

void ModuleManager::showAllValuesAndGetControllable(const StringArray & typeFilters, const StringArray& excludeTypeFilters, std::function<void(Controllable*)> returnFunc)
{
	PopupMenu menu;
	
	Array<Module *> mList = ModuleManager::getInstance()->getModuleList();
	int numItems = mList.size();

	const int maxValuesPerModule = 10000;

	for (int i = 0; i < numItems; ++i)
	{
		Module * m = mList[i];
		ControllableChooserPopupMenu *vCC = new ControllableChooserPopupMenu(&m->valuesCC, i* maxValuesPerModule, -1, typeFilters, excludeTypeFilters);
		getInstance()->modulesMenu.add(vCC);
		if (i == ModuleManager::getInstance()->items.size()) menu.addSeparator(); // Separator between user created module and special modules
		menu.addSubMenu(m->niceName, *vCC);
	}

	getInstance()->engineMenu.reset(new ControllableChooserPopupMenu(Engine::mainEngine, -1000000, -1, typeFilters, excludeTypeFilters));
	menu.addSubMenu("Generic", *getInstance()->engineMenu);

	
	menu.showMenuAsync(PopupMenu::Options(), [maxValuesPerModule, returnFunc](int result)
		{

			if (result < 0)
			{
				returnFunc(ModuleManager::getInstance()->engineMenu->getControllableForResult(result));
			}
			else
			{
				ControllableChooserPopupMenu* mm = ModuleManager::getInstance()->modulesMenu[(int)floorf(result / maxValuesPerModule)];
				returnFunc(mm->getControllableForResult(result));
			}
		}
	);

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

PopupMenu ModuleManager::getAllModulesCommandMenu(CommandContext context, bool multiplexMode)
{
	PopupMenu menu;
	for (int i = 0; i < items.size(); ++i) menu.addSubMenu(items[i]->niceName, items[i]->getCommandMenu(i * 1000,context));
	menu.addSeparator();
	menu.addSubMenu(StateManager::getInstance()->module->niceName, StateManager::getInstance()->module->getCommandMenu(-1000, context));
	menu.addSubMenu(ChataigneSequenceManager::getInstance()->module->niceName, ChataigneSequenceManager::getInstance()->module->getCommandMenu(-2000, context));
	menu.addSubMenu(CVGroupManager::getInstance()->module->niceName, CVGroupManager::getInstance()->module->getCommandMenu(-3000, context));
	menu.addSubMenu(static_cast<ChataigneEngine*>(Engine::mainEngine)->module->niceName, static_cast<ChataigneEngine*>(Engine::mainEngine)->module->getCommandMenu(-10000, context));
	if (multiplexMode)
	{
		menu.addSeparator();
		menu.addSubMenu(static_cast<ChataigneEngine*>(Engine::mainEngine)->multiplexModule->niceName, static_cast<ChataigneEngine*>(Engine::mainEngine)->multiplexModule->getCommandMenu(-20000, context));
	}
	return menu;
}

CommandDefinition * ModuleManager::getCommandDefinitionForItemID(int itemID, Module * lockedModule)
{

	if (itemID == 0) return nullptr;

	Module * m = lockedModule;

	if (itemID < -19000)
	{
		m = static_cast<ChataigneEngine*>(Engine::mainEngine)->multiplexModule.get();
		itemID += 20000;
	} else if (itemID < -9000)
	{
		m = static_cast<ChataigneEngine*>(Engine::mainEngine)->module.get();
		itemID += 10000;
	} else if (itemID < -2000)
	{
		m = CVGroupManager::getInstance()->module.get();
		itemID += 3000;
	} else if (itemID < -1000)
	{
		m = ChataigneSequenceManager::getInstance()->module.get();
		itemID += 2000;
	}else if (itemID < 0)
	{
		m = StateManager::getInstance()->module.get();
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
		mList.add(StateManager::getInstance()->module.get());
		mList.add(ChataigneSequenceManager::getInstance()->module.get());
		mList.add(CVGroupManager::getInstance()->module.get());
	}
	
	return mList;
}
