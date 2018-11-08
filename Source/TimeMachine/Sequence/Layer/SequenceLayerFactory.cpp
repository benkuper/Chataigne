/*
  ==============================================================================

    SequenceLayerFactory.cpp
    Created: 20 Nov 2016 3:09:30pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceLayerFactory.h"

#include "layers/trigger/TriggerLayer.h"
#include "layers/mapping/MappingLayer.h"
#include "layers/audio/AudioLayer.h"

juce_ImplementSingleton(SequenceLayerFactory)

SequenceLayerFactory::SequenceLayerFactory() {
	layerDefs.add(SequenceLayerDefinition::createDef("Trigger", &TriggerLayer::create));
	layerDefs.add(SequenceLayerDefinition::createDef("Mapping", &MappingLayer::create)->addParam("mode",MappingLayer::MODE_1D));
	//layerDefs.add(SequenceLayerDefinition::createDef("Mapping 2D", &MappingLayer::create)->addParam("mode", MappingLayer::MODE_2D));
	//layerDefs.add(SequenceLayerDefinition::createDef("Mapping 3D", &MappingLayer::create)->addParam("mode", MappingLayer::MODE_3D));
	layerDefs.add(SequenceLayerDefinition::createDef("Color", &MappingLayer::create)->addParam("mode", MappingLayer::MODE_COLOR));
	layerDefs.add(SequenceLayerDefinition::createDef("Audio", &AudioLayer::create));

	buildPopupMenu();
}

void SequenceLayerFactory::buildPopupMenu()
{
	int id = 1;
	for (auto &d : layerDefs)
	{
		menu.addItem(id, d->type);
		id++;
	}

}

inline SequenceLayer * SequenceLayerFactory::showCreateMenu(Sequence * sequence)
{
	if (layerDefs.size() == 1) return layerDefs[0]->createFunc(sequence, layerDefs[0]->params);

	int result = menu.show();
	if (result == 0) return nullptr;
	else
	{
		SequenceLayerDefinition * d = layerDefs[result - 1];//result 0 is no result
		return d->createFunc(sequence, d->params);
	}
}

SequenceLayerDefinition * SequenceLayerDefinition::createDef(const String & _type, std::function<SequenceLayer*(Sequence*, var)> createFunc)
{
	SequenceLayerDefinition * s = new SequenceLayerDefinition(_type, createFunc);
	return s;
}

SequenceLayerDefinition * SequenceLayerDefinition::addParam(const String & paramName, var value)
{
	params.getDynamicObject()->setProperty(paramName, value);
	return this;
}
