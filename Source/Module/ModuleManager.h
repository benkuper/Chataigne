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


class ModuleManager :
	public BaseManager<Module>
{
public:
	juce_DeclareSingleton(ModuleManager, true)

	ModuleManager();
	~ModuleManager();

	virtual Module * addItemFromData(var data, bool fromUndoableAction = false) override;
	
	Module * getModuleWithName(const String &moduleName);

	//Input values menu
	static Controllable * showAllValuesAndGetControllable(bool showTriggers, bool showParameters);
	static bool checkControllableIsAValue(Controllable * c);

	//Command menu
	PopupMenu getAllModulesCommandMenu(CommandContext context);
	CommandDefinition * getCommandDefinitionForItemID(int itemID, Module * lockedModule);

	Array<Module *> getModuleList(bool includeSpecialModules = true);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleManager)

};


#endif  // MODULEMANAGER_H_INCLUDED
