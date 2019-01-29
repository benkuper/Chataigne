/*
  ==============================================================================

    ModuleFactory.h
    Created: 8 Dec 2016 2:36:06pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MODULEFACTORY_H_INCLUDED
#define MODULEFACTORY_H_INCLUDED

#include "Module.h"

class ModuleDefinition
{
public:
	String menuPath;
	String moduleType;
	File moduleFolder; //for customModules
	var jsonData;
	std::function<Module*()> createFunc;

	ModuleDefinition(const String &menuPath, const String &type, std::function<Module*()> createFunc, var jsonData = var()) :
		menuPath(menuPath),
		moduleType(type),
		jsonData(jsonData),
		createFunc(createFunc)
	{}
};

class ModuleFactory
{
public:
	juce_DeclareSingleton(ModuleFactory, true);

	OwnedArray<ModuleDefinition> moduleDefs;
	HashMap<String, ModuleDefinition *> customModulesDefMap;

	PopupMenu menu;

	ModuleFactory();
	~ModuleFactory() {}


	void buildPopupMenu();

	void addCustomModules();
	void updateCustomModules();
	var getCustomModuleInfo(StringRef moduleName);
	File getFolderForCustomModule(StringRef moduleName) const;
	File getCustomModulesFolder() const;

	static Module* showCreateMenu();
	static Module * createModule(const String &moduleType);

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleFactory)
};




#endif  // MODULEFACTORY_H_INCLUDED
