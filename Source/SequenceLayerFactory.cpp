/*
  ==============================================================================

    SequenceLayerFactory.cpp
    Created: 20 Nov 2016 3:09:30pm
    Author:  Ben Kuper

  ==============================================================================
*/

#include "SequenceLayerFactory.h"

#include "TriggerLayer.h"
#include "MappingLayer.h"
#include "AudioLayer.h"

juce_ImplementSingleton(SequenceLayerFactory)

SequenceLayerFactory::SequenceLayerFactory() {
	layerDefs.add(new SequenceLayerDefinition("Triggers", &TriggerLayer::create));
	layerDefs.add(new SequenceLayerDefinition("Mapping", &MappingLayer::create));
	layerDefs.add(new SequenceLayerDefinition("Audio", &AudioLayer::create));

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
