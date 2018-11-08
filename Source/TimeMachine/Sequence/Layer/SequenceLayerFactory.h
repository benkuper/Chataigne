/*
  ==============================================================================

    SequenceLayerFactory.h
    Created: 20 Nov 2016 3:09:30pm
    Author:  Ben Kuper

  ==============================================================================
*/

#pragma once


#include "SequenceLayer.h"

class SequenceLayerDefinition
{
public:
	String type;
	std::function<SequenceLayer*(Sequence *, var)> createFunc;
	var params;

	SequenceLayerDefinition(const String &_type, std::function<SequenceLayer*(Sequence *, var)> createFunc) :
		type(_type),
		createFunc(createFunc)
	{
		params = var(new DynamicObject());
	}

	static SequenceLayerDefinition * createDef(const String &_type, std::function<SequenceLayer*(Sequence *, var)> createFunc);
	SequenceLayerDefinition * addParam(const String &paramName, var value);
};

class SequenceLayerFactory
{
public:
	juce_DeclareSingleton(SequenceLayerFactory,true)

	OwnedArray<SequenceLayerDefinition> layerDefs;
	PopupMenu menu;

	SequenceLayerFactory();
	~SequenceLayerFactory() {}

	void buildPopupMenu();

	SequenceLayer * showCreateMenu(Sequence * sequence);

	SequenceLayer * createSequenceLayer(Sequence * sequence, const String &inputType)
	{
		for (auto &d : layerDefs) if (d->type == inputType) return d->createFunc(sequence,d->params);
		return nullptr;
	}

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SequenceLayerFactory)
};