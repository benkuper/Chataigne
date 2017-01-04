/*
  ==============================================================================

    ModuleManagerUI.h
    Created: 8 Dec 2016 2:36:57pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEMANAGERUI_H_INCLUDED
#define MODULEMANAGERUI_H_INCLUDED

#include "BaseManagerShapeShifterUI.h"
#include "ModuleUI.h"
#include "ModuleManager.h"

class ModuleManagerUI :
	public BaseManagerShapeShifterUI<ModuleManager, Module, ModuleUI>
{
public:

	ModuleManagerUI(ModuleManager *_manager);
	~ModuleManagerUI();

	void showMenuAndAddItem(bool ,Point<int>) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManagerUI)
};




#endif  // MODULEMANAGERUI_H_INCLUDED
