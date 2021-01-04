/*
  ==============================================================================

    ModuleFactory.h
    Created: 8 Dec 2016 2:36:06pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "Module.h"

class ModuleDefinition :
	public FactoryDefinition<Module, std::function<Module* ()>>
{
public:
	ModuleDefinition(const String& menuPath, const String& type, std::function<Module* ()> createFunc, var customModuleData = var(), bool isCustomModule = false);
	
	File moduleFolder; //for customModules
	var customModuleData;
	bool isCustomModule;
	bool isLocalModule;

	Module* create() override;
};

class ModuleFactory :
	public Factory<Module>,
	public EngineListener
{
public:
	HashMap<String, ModuleDefinition*> customModulesDefMap;

	ModuleFactory();
	~ModuleFactory();

	void buildPopupMenu() override;
	Module * createFromMenuResult(int result) override;

	ModuleDefinition* getDefinitionForType(const String& moduleType);

	void addCustomModules();
	void addCustomModulesInFolder(File folder, bool isLocal);
	void updateCustomModules();
	var getCustomModuleInfo(StringRef moduleName);
	File getFolderForCustomModule(StringRef moduleName) const;
	File getCustomModulesFolder() const;

	Module* create(BaseFactoryDefinition<Module> * def) override;

	void startLoadFile() override;

};
