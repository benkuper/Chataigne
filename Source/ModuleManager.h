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
	
	Module * getModuleWithName(const String &moduleName);

	//Input values menu
	PopupMenu getAllModulesInputValuesMenu(bool parametersOnly);
	Controllable * getControllableForItemID(int itemID);
		
	//Command menu
	PopupMenu getAllModulesCommandMenu(CommandContext context);
	CommandDefinition * getCommandDefinitionForItemID(int itemID, Module * lockedModule);



	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManager)

};


#endif  // MODULEMANAGER_H_INCLUDED
