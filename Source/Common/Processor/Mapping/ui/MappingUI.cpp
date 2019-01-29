/*
  ==============================================================================

    MappingUI.cpp
    Created: 28 Oct 2016 8:06:18pm
    Author:  bkupe

  ==============================================================================
*/

#include "MappingUI.h"

MappingUI::MappingUI(Mapping * mapping) :
	ProcessorUI(mapping),
	mapping(mapping),
	outputParamUI(nullptr)
{
	mapping->addAsyncMappingListener(this);
	bgColor = MAPPING_COLOR;
	updateOutputParamUI();
}

MappingUI::~MappingUI()
{
	if (!inspectable.wasObjectDeleted()) mapping->removeAsyncMappingListener(this);
}

void MappingUI::updateOutputParamUI()
{
	if(outputParamUI != nullptr && mapping->outputParam != nullptr && outputParamUI->controllable == mapping->outputParam) return;

	if (outputParamUI != nullptr)
	{
		removeChildComponent(outputParamUI);
	}

	if (mapping->outputParam != nullptr)
	{
		outputParamUI = mapping->outputParam->createDefaultUI();
		addAndMakeVisible(outputParamUI);
	}

	resized();
}

void MappingUI::resizedInternalHeader(Rectangle<int> & r)
{
	ProcessorUI::resizedInternalHeader(r);
	if (outputParamUI != nullptr)
	{
		outputParamUI->setBounds(r.removeFromRight(120).reduced(2));
	}
}


void MappingUI::newMessage(const Mapping::MappingEvent & e)
{
	switch (e.type)
	{
	case Mapping::MappingEvent::OUTPUT_TYPE_CHANGED:
		updateOutputParamUI();
		break;
	}
}

