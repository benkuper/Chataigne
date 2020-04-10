/*
  ==============================================================================

    Mapping1DLayer.cpp
    Created: 25 Mar 2020 1:20:57pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping1DLayer.h"
#include "ui/Mapping1DLayerPanel.h"

Mapping1DLayer::Mapping1DLayer(Sequence* s, var params) :
    AutomationMappingLayer(getTypeString(), s, params)
{
    itemDataType = "MappingLayer";
    helpID = "MappingLayer";

    addChildControllableContainer(&automation1D);
    automation1D.recorder = &recorder;
    recorder.input->typesFilter.add(FloatParameter::getTypeStringStatic());
    recorder.input->typesFilter.add(IntParameter::getTypeStringStatic());
    recorder.input->typesFilter.add(BoolParameter::getTypeStringStatic());

    setupAutomation(&automation1D);
    setupMappingInputParameter(automation1D.value);
}

Mapping1DLayer::~Mapping1DLayer()
{
}

var Mapping1DLayer::getValueAtPosition(float position)
{
    return automation1D.getValueAtPosition(position);
}

void Mapping1DLayer::stopRecorderAndAddKeys()
{
    Array<AutomationRecorder::RecordValue> recordedValues = recorder.stopRecordingAndGetKeys();
    Array<Point<float>> points;
    for (auto& rv : recordedValues) points.add({ rv.time, (float)rv.value });
    automation1D.addFromPointsAndSimplify(points, true, true);
}

SequenceLayerPanel* Mapping1DLayer::getPanel()
{
    return new Mapping1DLayerPanel(this);
}
