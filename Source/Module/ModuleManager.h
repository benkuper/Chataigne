/*
  ==============================================================================

    ModuleManager.h
    Created: 8 Dec 2016 2:36:11pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module.h"


class ModuleManager :
	public BaseManager<Module>
{
public:
	juce_DeclareSingleton(ModuleManager, true)

		ModuleManager();
	~ModuleManager();

	virtual Module* addItemFromData(var data, bool addToUndo = false) override;
	virtual Array<Module*> addItemsFromData(var data, bool addToUndo = false) override;

	Module* getModuleWithName(const String& moduleName);

	void addItemInternal(Module* module, var data) override;

	//Input values menu
	static Controllable* showAllValuesAndGetControllable(bool showTriggers, bool showParameters);
	static bool checkControllableIsAValue(Controllable* c);

	template <class T>
	static Module* showAndGetModuleOfType()
	{
		PopupMenu menu;
		Array<Module*> validModules;
		for (auto& m : ModuleManager::getInstance()->items)
		{
			T* mt = dynamic_cast<T*>(m);
			if (mt == nullptr) continue;
			validModules.add(m);
			menu.addItem(validModules.indexOf(m) + 1, m->niceName);
		}

		int result = menu.show();
		if (result == 0) return nullptr;
		
		return validModules[result - 1];
	}

	//Command menu
	PopupMenu getAllModulesCommandMenu(CommandContext context);
	CommandDefinition* getCommandDefinitionForItemID(int itemID, Module* lockedModule);


	Array<Module*> getModuleList(bool includeSpecialModules = true);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManager)

};