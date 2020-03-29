/*
  ==============================================================================

    Mapping2DLayer.cpp
    Created: 23 Mar 2020 5:17:47pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping2DLayer.h"
#include "ui/Mapping2DLayerTimeline.h"
#include "ui/Mapping2DLayerPanel.h"

Mapping2DLayer::Mapping2DLayer(Sequence* s, var params) :
	AutomationMappingLayer(getTypeString(), s, params)
{
	saveAndLoadRecursiveData = true;

	itemDataType = "Mapping2DLayer";
	helpID = "Mapping2DLayer";

	addChildControllableContainer(&curve);
	
	curve.setControlMode(Curve2D::AUTOMATION);

	setupAutomation((Automation*)curve.position->automation->automationContainer);
	setupMappingInputParameter(curve.value);

	uiHeight->setValue(200);
}

Mapping2DLayer::~Mapping2DLayer()
{
}

void Mapping2DLayer::addDefaultContent()
{
	automation->addKey(0, 0, false);
	automation->addKey(sequence->totalTime->floatValue(), 1, false);
}


var Mapping2DLayer::getValueAtPosition(float position)
{
	Point<float> p = curve.getValueAtNormalizedPosition(automation->getNormalizedValueAtPosition(position));
	var result;
	result.append(p.x);
	result.append(p.y);
	return result;
}

SequenceLayerPanel* Mapping2DLayer::getPanel()
{
	return new Mapping2DLayerPanel(this);
}

SequenceLayerTimeline* Mapping2DLayer::getTimelineUI()
{
	return new Mapping2DTimeline(this);
}
