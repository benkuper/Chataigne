/*
  ==============================================================================

    ConditionFactory.h
    Created: 21 Feb 2017 11:56:12am
    Author:  Ben

  ==============================================================================
*/

#ifndef CONDITIONFACTORY_H_INCLUDED
#define CONDITIONFACTORY_H_INCLUDED

#include "Condition.h"

class ConditionDefinition
{
public:
	String menuPath;
	String conditionType;
	std::function<Condition*()> createFunc;

	ConditionDefinition(const String &menuPath, const String &type, std::function<Condition*()> createFunc) :
		menuPath(menuPath),
		conditionType(type),
		createFunc(createFunc)
	{}
};

class ConditionFactory
{
public:
	juce_DeclareSingleton(ConditionFactory, true);

	OwnedArray<ConditionDefinition> conditionDefs;
	PopupMenu menu;

	ConditionFactory();
	~ConditionFactory() {}

	void buildPopupMenu();

	static Condition* showCreateMenu()
	{
		int result = getInstance()->menu.show();
		if (result == 0) return nullptr;
		else
		{
			ConditionDefinition * d = getInstance()->conditionDefs[result - 1];//result 0 is no result
			return d->createFunc();
		}
	}

	static Condition * createModule(const String &conditionType)
	{
		for (auto &d : getInstance()->conditionDefs) if (d->conditionType == conditionType) return d->createFunc();
		return nullptr;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConditionFactory)
};




#endif  // CONDITIONFACTORY_H_INCLUDED
