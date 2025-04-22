/*
  ==============================================================================

	ModuleManager.h
	Created: 8 Dec 2016 2:36:11pm
	Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleFactory;

class ModuleManager :
	public Manager<Module>
{
public:

	ModuleManager(const String& name = "Items");
	~ModuleManager();


	OwnedArray<ControllableChooserPopupMenu> modulesMenu;
	std::unique_ptr<ControllableChooserPopupMenu> engineMenu;


	void addItemInternal(Module* module, var data) override;


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManager)

};

class RootModuleManager :
	public ModuleManager
{
public:
	juce_DeclareSingleton(RootModuleManager, true);

	RootModuleManager();
	~RootModuleManager() {}


	Array<Module*> getModuleList(bool includeSpecialModules = true);
	Module* getModuleWithName(const String& moduleName, bool recursive = true);

	//Input values menu
	static void showAllValuesAndGetControllable(const StringArray& typeFilters, const StringArray& excludeTypeFilters, ControllableContainer* startFromCC, std::function<void(Controllable*)> returnFunc);
	static bool checkControllableIsAValue(Controllable* c);

	template <class T>
	static void showAndGetModuleOfType(ControllableContainer* startFromCC, std::function<void(ControllableContainer*)> returnFunc)
	{
		PopupMenu menu;
		Array<Module*> validModules;
		for (auto& m : RootModuleManager::getInstance()->items)
		{
			T* mt = dynamic_cast<T*>(m);
			if (mt == nullptr) continue;
			validModules.add(m);
			menu.addItem(validModules.indexOf(m) + 1, m->niceName);
		}

		menu.showMenuAsync(PopupMenu::Options(), [validModules, returnFunc](int result)
			{

				if (result == 0) return;
				returnFunc(validModules[result - 1]);
			}
		);
	}

	//Command menu
	PopupMenu getAllModulesCommandMenu(CommandContext context, bool multiplexMode = false);
	CommandDefinition* getCommandDefinitionForItemID(int itemID, Module* lockedModule);
};