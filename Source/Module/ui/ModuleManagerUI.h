/*
  ==============================================================================

    ModuleManagerUI.h
    Created: 8 Dec 2016 2:36:57pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEMANAGERUI_H_INCLUDED
#define MODULEMANAGERUI_H_INCLUDED

#include "ModuleUI.h"
#include "../ModuleManager.h"

class ModuleManagerUI :
	public BaseManagerShapeShifterUI<ModuleManager, Module, ModuleUI>
{
public:

	ModuleManagerUI(const String &contentName, ModuleManager *_manager);
	~ModuleManagerUI();

	void showMenuAndAddItem(bool ,Point<int>) override;

	static ModuleManagerUI * create(const String &contentName) { return new ModuleManagerUI(contentName, ModuleManager::getInstance()); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManagerUI)
};




#endif  // MODULEMANAGERUI_H_INCLUDED
