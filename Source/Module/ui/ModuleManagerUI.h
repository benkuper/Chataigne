/*
  ==============================================================================

    ModuleManagerUI.h
    Created: 8 Dec 2016 2:36:57pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "ModuleUI.h"
#include "../ModuleManager.h"

class ModuleManagerUI :
	public BaseManagerShapeShifterUI<ModuleManager, Module, ModuleUI>
{
public:

	ModuleManagerUI(const String &contentName, ModuleManager *_manager);
	~ModuleManagerUI();

	ModuleUI* createUIForItem(Module* item) override;
	void showMenuAndAddItem(bool ,Point<int>) override;

	static ModuleManagerUI * create(const String &contentName) { return new ModuleManagerUI(contentName, ModuleManager::getInstance()); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManagerUI)
};