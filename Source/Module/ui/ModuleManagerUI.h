/*
  ==============================================================================

    ModuleManagerUI.h
    Created: 8 Dec 2016 2:36:57pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleManagerUI :
	public BaseManagerShapeShifterUI<ModuleManager, Module, ModuleUI>
{
public:

	ModuleManagerUI(const String &contentName, ModuleManager *_manager);
	~ModuleManagerUI();

	ModuleUI* createUIForItem(Module* item) override;
	void addItemFromMenu(Module* m, bool fromAddButton, Point<int> pos) override;

	static ModuleManagerUI * create(const String &contentName) { return new ModuleManagerUI(contentName, ModuleManager::getInstance()); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManagerUI)
};