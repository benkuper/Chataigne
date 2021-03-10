/*
  ==============================================================================

    Mapping1DLayer.h
    Created: 25 Mar 2020 1:20:57pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Mapping1DLayer :
    public AutomationMappingLayer
{
public:
    Mapping1DLayer(Sequence* s, var params);
    ~Mapping1DLayer();

    Automation automation1D;

    virtual var getValueAtPosition(float position) override;

    virtual void stopRecorderAndAddKeys() override;

    SequenceLayerPanel* getPanel() override;

    virtual String getTypeString() const override { return getTypeStringStatic(); }
    static const String getTypeStringStatic() { return "Mapping"; }

    static Mapping1DLayer* create(Sequence* sequence, var params) { return new Mapping1DLayer(sequence, params); }

};