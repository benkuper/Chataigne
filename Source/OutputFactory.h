/*
  ==============================================================================

    OutputFactory.h
    Created: 4 Nov 2016 12:28:44am
    Author:  bkupe

  ==============================================================================
*/

#ifndef OUTPUTFACTORY_H_INCLUDED
#define OUTPUTFACTORY_H_INCLUDED

#include "Output.h"

class OutputDefinition
{
public:
	String menuPath;
	String outputType;
	std::function<Output*()> createFunc;

	OutputDefinition(const String &menuPath, const String &type, std::function<Output*()> createFunc) :
		menuPath(menuPath),
		outputType(type),
		createFunc(createFunc)
	{}
};

class OutputFactory
{
public:
	juce_DeclareSingleton(OutputFactory, true);

	OwnedArray<OutputDefinition> outputDefs;
	PopupMenu menu;

	OutputFactory();
	~OutputFactory() {}

	void buildPopupMenu();

	static Output * showCreateMenu()
	{
		int result = getInstance()->menu.show();
		if (result == 0) return nullptr;
		else
		{
			OutputDefinition * d = getInstance()->outputDefs[result - 1];//result 0 is no result
			return d->createFunc();
		}
	}

	static Output * createOutput(const String &outputType)
	{
		for (auto &d : getInstance()->outputDefs) if (d->outputType == outputType) return d->createFunc();
		return nullptr;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OutputFactory)
};



#endif  // OUTPUTFACTORY_H_INCLUDED
