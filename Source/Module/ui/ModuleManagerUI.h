/*
  ==============================================================================

    ModuleManagerUI.h
    Created: 8 Dec 2016 2:36:57pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleManagerUI :
	public ManagerUI<ModuleManager, Module, ModuleUI, ItemBaseGroup<Module>, ModuleGroupUI>
{
public:

	ModuleManagerUI(const String& contentName, ModuleManager* _manager);
	virtual ~ModuleManagerUI();

	ModuleUI* createUIForItem(Module* item) override;
	void addItemFromMenu(Module* m, bool fromAddButton, Point<int> pos) override;

};

class RootModuleManagerUI :
	public ModuleManagerUI,
	public ShapeShifterContent
{
public:

	RootModuleManagerUI(const String &contentName, ModuleManager *_manager);
	~RootModuleManagerUI();

	static RootModuleManagerUI* create(const String& contentName) { return new RootModuleManagerUI(contentName, RootModuleManager::getInstance()); }


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(RootModuleManagerUI)
};