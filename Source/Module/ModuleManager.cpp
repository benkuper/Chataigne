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

juce_ImplementSingleton(RootModuleManager)

ModuleManager::ModuleManager(const String& name) :
	Manager<Module>(name, true)
{
	itemDataType = "Module";
	helpID = "Modules";
	showInspectorOnSelect = false;

	managerFactory = ModuleFactory::getInstance();


}

ModuleManager::~ModuleManager()
{
}


ItemBaseGroup<Module>* ModuleManager::createGroup() {
	return new ItemBaseGroup<Module>(new ModuleManager("Items"));
}

void ModuleManager::addItemInternal(Module* module, var data)
{
	module->templateManager->setupDefinitionsFromModule();
}



RootModuleManager::RootModuleManager() :
	ModuleManager("Modules")
{
	ControllableFactory::getInstance()->controllableDefs.add(new ControllableDefinition(MIDIValueParameter::getTypeStringStatic(), &MIDIValueParameter::create, true));
	ControllableFactory::getInstance()->controllableDefs.add(new ControllableDefinition(DMXValueParameter::getTypeStringStatic(), &DMXValueParameter::create, true));
}


Array<Module*> RootModuleManager::getModuleList(bool includeSpecialModules)
{
	Array<Module*> mList = RootModuleManager::getInstance()->getItems();
	for (auto& m : RootModuleManager::getInstance()->items) mList.add(m);

	if (includeSpecialModules)
	{
		mList.add(StateManager::getInstance()->module.get());
		mList.add(ChataigneSequenceManager::getInstance()->module.get());
		mList.add(CVGroupManager::getInstance()->module.get());
	}

	return mList;
}

Module* RootModuleManager::getModuleWithName(const String& moduleName, bool recursive)
{
	//DBG("get Module with name : " << moduleName << " / " << StateManager::getInstance()->shortName);
	if (moduleName == StateManager::getInstance()->module->shortName) return StateManager::getInstance()->module.get();
	if (moduleName == ChataigneSequenceManager::getInstance()->module->shortName) return ChataigneSequenceManager::getInstance()->module.get();
	if (moduleName == CVGroupManager::getInstance()->module->shortName) return CVGroupManager::getInstance()->module.get();
	if (moduleName == static_cast<ChataigneEngine*>(Engine::mainEngine)->module->shortName) return static_cast<ChataigneEngine*>(Engine::mainEngine)->module.get();
	if (moduleName == static_cast<ChataigneEngine*>(Engine::mainEngine)->multiplexModule->shortName) return static_cast<ChataigneEngine*>(Engine::mainEngine)->multiplexModule.get();
	else return getItemWithName(moduleName, true);
}

void RootModuleManager::showAllValuesAndGetControllable(const StringArray& typeFilters, const StringArray& excludeTypeFilters, ControllableContainer* startFromCC, std::function<void(Controllable*)> returnFunc)
{

	if (startFromCC != nullptr)
	{
		ControllableChooserPopupMenu* cMenu = new ControllableChooserPopupMenu(startFromCC, 0, -1, typeFilters, excludeTypeFilters);
		cMenu->showAndGetControllable(returnFunc, true);
		return;
	}

	PopupMenu menu;
	Array<Module*> mList = RootModuleManager::getInstance()->getModuleList();
	int numItems = mList.size();

	const int maxValuesPerModule = 100000;

	getInstance()->modulesMenu.clear();

	for (int i = 0; i < numItems; ++i)
	{
		Module* m = mList[i];
		ControllableChooserPopupMenu* vCC = new ControllableChooserPopupMenu(&m->valuesCC, i * maxValuesPerModule, -1, typeFilters, excludeTypeFilters);
		getInstance()->modulesMenu.add(vCC);

		if(m == StateManager::getInstance()->module.get()) menu.addSeparator(); // Separator between user created module and special modules

		String crumb = m->getBreadCrumb().joinIntoString(" > ");
		menu.addSubMenu(crumb, *vCC);
	}

	getInstance()->engineMenu.reset(new ControllableChooserPopupMenu(Engine::mainEngine, -10000000, -1, typeFilters, excludeTypeFilters));
	menu.addSubMenu("Generic", *getInstance()->engineMenu);


	menu.showMenuAsync(PopupMenu::Options(), [maxValuesPerModule, returnFunc](int result)
		{

			if (result < 0)
			{
				returnFunc(RootModuleManager::getInstance()->engineMenu->getControllableForResult(result));
			}
			else
			{
				ControllableChooserPopupMenu* mm = RootModuleManager::getInstance()->modulesMenu[(int)floorf(result / maxValuesPerModule)];
				returnFunc(mm->getControllableForResult(result));
			}
		}
	);

}

bool RootModuleManager::checkControllableIsAValue(Controllable* c)
{
	ControllableContainer* cc = c->parentContainer;
	while (cc != nullptr)
	{
		Module* m = dynamic_cast<Module*>(cc); //If controllable is child of a module

		if (c->parentContainer == m || c->parentContainer == &m->moduleParams) return false; //If controllable is direct child of this module, or child 

		if (m != nullptr)
		{
			ControllableContainer* vcc = c->parentContainer;
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

PopupMenu RootModuleManager::getAllModulesCommandMenu(CommandContext context, bool multiplexMode)
{
	PopupMenu menu;
	Array<Module*> mList = RootModuleManager::getInstance()->getModuleList(false);
	for (int i = 0; i < mList.size(); ++i) menu.addSubMenu(mList[i]->getBreadCrumb().joinIntoString(" > "), mList[i]->getCommandMenu(i * 1000, context));

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

CommandDefinition* RootModuleManager::getCommandDefinitionForItemID(int itemID, Module* lockedModule)
{
	if (itemID == 0) return nullptr;

	Module* m = lockedModule;

	if (itemID < -19000)
	{
		m = static_cast<ChataigneEngine*>(Engine::mainEngine)->multiplexModule.get();
		itemID += 20000;
	}
	else if (itemID < -9000)
	{
		m = static_cast<ChataigneEngine*>(Engine::mainEngine)->module.get();
		itemID += 10000;
	}
	else if (itemID < -2000)
	{
		m = CVGroupManager::getInstance()->module.get();
		itemID += 3000;
	}
	else if (itemID < -1000)
	{
		m = ChataigneSequenceManager::getInstance()->module.get();
		itemID += 2000;
	}
	else if (itemID < 0)
	{
		m = StateManager::getInstance()->module.get();
		itemID += 1000;
	}
	else if (m == nullptr)
	{
		Array<Module*> mList = RootModuleManager::getInstance()->getModuleList(false);

		int moduleIndex = (int)floor(itemID / 1000);
		m = mList[moduleIndex];
	}


	if (m == nullptr) return nullptr;

	int commandIndex = itemID % 1000 - 1;
	return m->getCommandDefinitionForItemID(commandIndex);
}
