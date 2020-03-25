/*
  ==============================================================================

    ColorMappingLayer.h
    Created: 25 Mar 2020 12:49:54pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


#include "../MappingLayer.h"

class ColorMappingLayer :
    public MappingLayer
{
public:
    ColorMappingLayer(Sequence* s, var params);
    ~ColorMappingLayer();

    GradientColorManager colorManager;

    void addDefaultContent() override;

    var getValueAtPosition(float position) override;
    void selectAll(bool addToSelection = false) override;

    virtual void sequenceCurrentTimeChanged(Sequence* s, float prevTime, bool seeking) override;
    virtual void sequenceTotalTimeChanged(Sequence* s) override;

    SequenceLayerPanel* getPanel() override;
    SequenceLayerTimeline* getTimelineUI() override;

    virtual String getTypeString() const override { return getTypeStringStatic(); }
    static const String getTypeStringStatic() { return "Color"; }
    static ColorMappingLayer* create(Sequence* sequence, var params) { return new ColorMappingLayer(sequence, params); }
};
