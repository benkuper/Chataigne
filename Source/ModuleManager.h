/*
  ==============================================================================

    ModuleManager.h
    Created: 8 Dec 2016 2:36:11pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEMANAGER_H_INCLUDED
#define MODULEMANAGER_H_INCLUDED

#include "Module.h"
#include "BaseManager.h"

class ModuleManager :
	public BaseManager<Module>
{
public:
	juce_DeclareSingleton(ModuleManager, true)

	ModuleManager();
	~ModuleManager();

	virtual void addItemFromData(var data) override;

	PopupMenu getAllModulesCommandMenu();
	CommandDefinition * getCommandDefinitionForItemID(int id);


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManager)

};


#endif  // MODULEMANAGER_H_INCLUDED
