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
	BaseManager(name)
{
	itemDataType = "CVGroup";
	module.reset(new CustomVariablesModule(this));
	addChildControllableContainer(module.get());
}

CVGroupManager::~CVGroupManager()
{
}

void CVGroupManager::showMenuAndGetContainer(std::function<void(ControllableContainer*)> returnFunc)
{
	PopupMenu menu;
	int numItems = CVGroupManager::getInstance()->items.size();

	for (int i = 0; i < numItems; ++i)
	{
		CVGroup* g = CVGroupManager::getInstance()->items[i];
		menu.addItem(i + 1, g->niceName);
	}

	menu.showMenuAsync(PopupMenu::Options(), [returnFunc](int result)
		{
			if (result > 0)
			{
				returnFunc(CVGroupManager::getInstance()->items[result - 1]);
			}
		}
	);
}

void CVGroupManager::showMenuAndGetVariable(const StringArray& typeFilters, const StringArray& excludeTypeFilters, std::function<void(Controllable*)> returnFunc)
{
	PopupMenu menu;
	int numItems = CVGroupManager::getInstance()->items.size();
	Array<Controllable*> controllableRefs;

	int itemID = 1;
	for (int i = 0; i < numItems; ++i)
	{
		CVGroup* g = CVGroupManager::getInstance()->items[i];

		PopupMenu sMenu;
		int numVariables = g->values.items.size();
		for (int j = 0; j < numVariables; j++)
		{
			controllableRefs.add(g->values.items[j]->controllable);
			sMenu.addItem(itemID, g->values.items[j]->controllable->niceName);
			itemID++;
		}

		menu.addSubMenu(g->niceName, sMenu);
	}

	menu.showMenuAsync(PopupMenu::Options(), [returnFunc, controllableRefs](int result)
		{
			if (result > 0)
			{
				returnFunc(controllableRefs[result - 1]);
			}
		}
	);
}

void CVGroupManager::showMenuAndGetPreset(std::function<void(ControllableContainer*)> returnFunc)
{
	PopupMenu menu;
	int numItems = CVGroupManager::getInstance()->items.size();
	Array<ControllableContainer*> presetRefs;

	int itemID = 1;
	for (int i = 0; i < numItems; ++i)
	{
		CVGroup* g = CVGroupManager::getInstance()->items[i];

		PopupMenu sMenu;
		int numVariables = g->pm->items.size();
		for (int j = 0; j < numVariables; j++)
		{
			presetRefs.add(g->pm->items[j]);
			sMenu.addItem(itemID, g->pm->items[j]->niceName);
			itemID++;
		}

		menu.addSubMenu(g->niceName, sMenu);
	}

	menu.showMenuAsync(PopupMenu::Options(), [returnFunc, presetRefs](int result)
		{
			if (result > 0)
			{
				returnFunc(presetRefs[result - 1]);
			}
		}
	);
}

void CVGroupManager::showMenuAndGetGroup(std::function<void(ControllableContainer*)> returnFunc)
{
	PopupMenu menu;
	int numItems = CVGroupManager::getInstance()->items.size();
	for (int i = 0; i < numItems; ++i)
	{
		menu.addItem(i + 1, CVGroupManager::getInstance()->items[i]->niceName);
	}

	menu.showMenuAsync(PopupMenu::Options(), [returnFunc](int result)
		{
			if (result > 0)
			{
				returnFunc(CVGroupManager::getInstance()->items[result - 1]);
			}
		}
	);
}
