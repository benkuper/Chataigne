/*
  ==============================================================================

    MappingFilterFactory.h
    Created: 4 Feb 2017 5:25:21pm
    Author:  Ben

  ==============================================================================
*/

#ifndef MAPPINGFILTERFACTORY_H_INCLUDED
#define MAPPINGFILTERFACTORY_H_INCLUDED

#include "MappingFilter.h"

class MappingFilterDefinition
{
public:
	String menuPath;
	String type;
	std::function<MappingFilter*()> createFunc;

	MappingFilterDefinition(const String &menuPath, const String &type, std::function<MappingFilter*()> createFunc) :
		menuPath(menuPath),
		type(type),
		createFunc(createFunc)
	{}
};


class MappingFilterFactory
{
public:
	juce_DeclareSingleton(MappingFilterFactory, true);

	OwnedArray<MappingFilterDefinition> defs;
	PopupMenu menu;

	MappingFilterFactory();
	~MappingFilterFactory() {}

	void buildPopupMenu();

	static MappingFilter* showCreateMenu()
	{
		DBG("Show create menu");
		int result = getInstance()->menu.show();
		if (result == 0) return nullptr;
		else
		{
			MappingFilterDefinition * d = getInstance()->defs[result - 1];//result 0 is no result
			return d->createFunc();
		}
	}

	static MappingFilter * createModule(const String &moduleType)
	{
		for (auto &d : getInstance()->defs) if (d->type == moduleType) return d->createFunc();
		return nullptr;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MappingFilterFactory)
};



#endif  // MAPPINGFILTERFACTORY_H_INCLUDED
