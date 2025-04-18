/*
  ==============================================================================

	CVGroupManager.cpp
	Created: 17 Feb 2018 10:16:11am
	Author:  Ben

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

juce_ImplementSingleton(CVGroupManager)

CVGroupManager::CVGroupManager(const String& name) :
	Manager(name)
{
	itemDataType = "CVGroup";
	module.reset(new CustomVariablesModule(this));
	addChildControllableContainer(module.get());
}

CVGroupManager::~CVGroupManager()
{
}

void CVGroupManager::showMenuAndGetContainer(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)> returnFunc)
{
	PopupMenu menu;
	int numItems = CVGroupManager::getInstance()->getNumItems();

	for (int i = 0; i < numItems; ++i)
	{
		CVGroup* g = CVGroupManager::getInstance()->getItemAt(i);
		menu.addItem(i + 1, g->niceName);
	}

	menu.showMenuAsync(PopupMenu::Options(), [returnFunc](int result)
		{
			if (result > 0)
			{
				returnFunc(CVGroupManager::getInstance()->getItemAt(result - 1));
			}
		}
	);
}

void CVGroupManager::showMenuAndGetVariable(const StringArray& typeFilters, const StringArray& excludeTypeFilters, ControllableContainer* startFromCC, std::function<void(Controllable*)> returnFunc)
{

	PopupMenu menu;
	Array<Controllable*> controllableRefs;

	auto getMenuForGroup = [&controllableRefs](CVGroup* g)
	{
		PopupMenu sMenu;
		int numVariables = g->values.getNumItems();
		for (int j = 0; j < numVariables; j++)
		{
			controllableRefs.add(g->values.getItemAt(j)->controllable);
			sMenu.addItem(controllableRefs.size(), g->values.getItemAt(j)->controllable->niceName);
		}

		return sMenu;
	};


	if (CVGroup* g = dynamic_cast<CVGroup*>(startFromCC)) menu = getMenuForGroup(g);
	else CVGroupManager::getInstance()->callFunctionOnItems([&](auto g) { menu.addSubMenu(g->niceName, getMenuForGroup(g)); });

	menu.showMenuAsync(PopupMenu::Options(), [returnFunc, controllableRefs](int result)
		{
			if (result > 0)
			{
				returnFunc(controllableRefs[result - 1]);
			}
		}
	);
}

void CVGroupManager::showMenuAndGetPreset(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)> returnFunc)
{
	PopupMenu menu;
	Array<ControllableContainer*> presetRefs;

	auto getMenuForGroup = [&presetRefs](CVGroup* g)
	{
		PopupMenu sMenu;
		int numVariables = g->pm->getNumItems();
		for (int j = 0; j < numVariables; j++)
		{
			presetRefs.add(g->pm->getItemAt(j));
			sMenu.addItem(presetRefs.size(), g->pm->getItemAt(j)->niceName);
		}

		return sMenu;
	};


	if (CVGroup* g = dynamic_cast<CVGroup*>(startFromCC)) menu = getMenuForGroup(g);
	else CVGroupManager::getInstance()->callFunctionOnItems([&](auto g) {menu.addSubMenu(g->niceName, getMenuForGroup(g)); });

	menu.showMenuAsync(PopupMenu::Options(), [returnFunc, presetRefs](int result)
		{
			if (result > 0)
			{
				returnFunc(presetRefs[result - 1]);
			}
		}
	);
}

void CVGroupManager::showMenuAndGetGroup(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)> returnFunc)
{
	PopupMenu menu;
	int numItems = CVGroupManager::getInstance()->getNumItems();
	for (int i = 0; i < numItems; ++i)
	{
		menu.addItem(i + 1, CVGroupManager::getInstance()->getItemAt(i)->niceName);
	}

	menu.showMenuAsync(PopupMenu::Options(), [returnFunc](int result)
		{
			if (result > 0)
			{
				returnFunc(CVGroupManager::getInstance()->getItemAt(result - 1));
			}
		}
	);
}
