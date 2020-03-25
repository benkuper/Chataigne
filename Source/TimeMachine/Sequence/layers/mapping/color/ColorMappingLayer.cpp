/*
  ==============================================================================

    ColorMappingLayer.cpp
    Created: 25 Mar 2020 12:49:54pm
    Author:  bkupe

  ==============================================================================
*/

#include "ColorMappingLayer.h"
#include "ui/ColorMappingLayerTimeline.h"
#include "../ui/MappingLayerPanel.h"

ColorMappingLayer::ColorMappingLayer(Sequence* s, var params) :
    MappingLayer(getTypeString(), s, params),
    colorManager(sequence->totalTime->floatValue(),false, false)
{
    itemDataType = "ColorLayer";
    helpID = "ColorLayer";

    colorManager.allowKeysOutside = false;
    colorManager.hideInEditor = true;
    colorManager.setLength(sequence->totalTime->floatValue());
    addChildControllableContainer(&colorManager);

    setupMappingInputParameter(colorManager.currentColor);

    uiHeight->setValue(60);
}

ColorMappingLayer::~ColorMappingLayer()
{
}

void ColorMappingLayer::addDefaultContent()
{
    colorManager.addColorAt(sequence->totalTime->floatValue() / 5, Colours::green);
    colorManager.addColorAt(sequence->totalTime->floatValue() * 2 / 5, Colours::yellow);
    colorManager.addColorAt(sequence->totalTime->floatValue() * 3 / 5, Colours::red)->interpolation->setValueWithData(GradientColor::NONE);
    colorManager.addColorAt(sequence->totalTime->floatValue() * 4 / 5, Colours::blue);
}

var ColorMappingLayer::getValueAtPosition(float position)
{
    Colour c = colorManager.getColorForPosition(position);
    var result;
    result.append(c.getFloatRed());
    result.append(c.getFloatGreen());
    result.append(c.getFloatBlue());
    result.append(c.getFloatAlpha());
    return result;
}

void ColorMappingLayer::selectAll(bool addToSelection)
{
    deselectThis(colorManager.items.size() == 0);
    colorManager.askForSelectAllItems(addToSelection);
}

void ColorMappingLayer::sequenceCurrentTimeChanged(Sequence* s, float prevTime, bool seeking)
{
    colorManager.position->setValue(sequence->currentTime->floatValue());
}

void ColorMappingLayer::sequenceTotalTimeChanged(Sequence* s)
{
    colorManager.setLength(sequence->totalTime->floatValue());
}

SequenceLayerPanel* ColorMappingLayer::getPanel()
{
    return new MappingLayerPanel(this);
}

SequenceLayerTimeline* ColorMappingLayer::getTimelineUI()
{
    return new ColorMappingLayerTimeline(this);
}
