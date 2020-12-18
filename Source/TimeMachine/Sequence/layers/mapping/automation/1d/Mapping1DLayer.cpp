/*
  ==============================================================================

    Mapping1DLayer.cpp
    Created: 25 Mar 2020 1:20:57pm
    Author:  bkupe

  ==============================================================================
*/

#include "Mapping1DLayer.h"
#include "ui/Mapping1DLayerPanel.h"

#include "Module/modules/midi/MIDIModule.h"
#include "Module/modules/dmx/DMXModule.h"

Mapping1DLayer::Mapping1DLayer(Sequence* s, var params) :
    AutomationMappingLayer(getTypeString(), s, params)
{
    helpID = "MappingLayer";

    addChildControllableContainer(&automation1D, false, 0);
    automation1D.recorder = &recorder;
    recorder.input->typesFilter.add(FloatParameter::getTypeStringStatic());
    recorder.input->typesFilter.add(IntParameter::getTypeStringStatic());
    recorder.input->typesFilter.add(BoolParameter::getTypeStringStatic());

    //maybe need to find a way that those values are automatically recognized because they derive from IntParameter
    recorder.input->typesFilter.add(MIDIValueParameter::getTypeStringStatic());
    recorder.input->typesFilter.add(DMXValueParameter::getTypeStringStatic());

    setupAutomation(&automation1D);
    automation1D.hideInEditor = true;
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
    AutomationRecorder::SimplificationMethod sm = recorder.simplificationMethod->getValueDataAsEnum<AutomationRecorder::SimplificationMethod>();
    if (sm == AutomationRecorder::SIMPL_BEZIER) automation1D.addFromPointsAndSimplifyBezier(points, true, true);
    if (sm == AutomationRecorder::SIMPL_LINEAR) automation1D.addFromPointsAndSimplifyLinear(points, recorder.simplificationTolerance->floatValue(), true, true);
    if (sm == AutomationRecorder::SIMPL_LINEAR_INTERACTIVE) automation1D.launchInteractiveSimplification(points);

}

SequenceLayerPanel* Mapping1DLayer::getPanel()
{
    return new Mapping1DLayerPanel(this);
}
