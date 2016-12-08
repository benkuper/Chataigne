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
	std::function<Module*()> createFunc;

	ModuleDefinition(const String &menuPath, const String &type, std::function<Module*()> createFunc) :
		menuPath(menuPath),
		moduleType(type),
		createFunc(createFunc)
	{}
};

class ModuleFactory
{
public:
	juce_DeclareSingleton(ModuleFactory, true);

	OwnedArray<ModuleDefinition> moduleDefs;
	PopupMenu menu;

	ModuleFactory();
	~ModuleFactory() {}

	void buildPopupMenu();

	static Module* showCreateMenu()
	{
		int result = getInstance()->menu.show();
		if (result == 0) return nullptr;
		else
		{
			ModuleDefinition * d = getInstance()->moduleDefs[result - 1];//result 0 is no result
			return d->createFunc();
		}
	}

	static Module * createModule(const String &moduleType)
	{
		for (auto &d : getInstance()->moduleDefs) if (d->moduleType == moduleType) return d->createFunc();
		return nullptr;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModuleFactory)
};




#endif  // MODULEFACTORY_H_INCLUDED
