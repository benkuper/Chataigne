/*
  ==============================================================================

    ModuleFactory.h
    Created: 8 Dec 2016 2:36:06pm
    Author:  Ben

  ==============================================================================
*/

#pragma once

class ModuleDefinition :
	public FactoryDefinition<Module, std::function<Module* ()>>
{
public:
	ModuleDefinition(const String& menuPath, const String& type, std::function<Module* ()> createFunc, var customModuleData = var(), bool isCustomModule = false);
	
	File moduleFolder; //for customModules
	var customModuleData;
	bool isCustomModule;
	bool isLocalModule;
	bool newVersionAvailable;

	Module* create() override;
};

class ModuleFactory :
	public Factory<Module>
{
public:
	HashMap<String, ModuleDefinition*> customModulesDefMap;

	ModuleFactory();
	~ModuleFactory();

	void buildPopupMenu(int startOffset = 0) override;
	Module * createFromMenuResult(int result) override;

	ModuleDefinition* getDefinitionForType(const String& moduleType);

	void addCustomModules(bool log = true);
	void addCustomModulesInFolder(File folder, bool isLocal, bool log = true);
	void updateCustomModules(bool log = true);
	var getCustomModuleInfo(StringRef moduleName);
	void setModuleNewVersionAvailable(StringRef moduleName, bool newVersionAvailable);
	File getFolderForCustomModule(StringRef moduleName) const;
	File getCustomModulesFolder() const;

	Module* create(BaseFactoryDefinition<Module> * def) override;
};
