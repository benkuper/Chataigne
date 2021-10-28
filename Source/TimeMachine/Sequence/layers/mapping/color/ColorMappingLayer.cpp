/*
  ==============================================================================

    ColorMappingLayer.cpp
    Created: 25 Mar 2020 12:49:54pm
    Author:  bkupe

  ==============================================================================
*/

ColorMappingLayer::ColorMappingLayer(Sequence* s, var params) :
    MappingLayer(getTypeString(), s, params),
    colorManager(sequence->totalTime->floatValue(),false, false)
{
    helpID = "ColorLayer";

    colorManager.allowKeysOutside = false;
    colorManager.hideInEditor = true;
    colorManager.setLength(sequence->totalTime->floatValue());
    colorManager.length->setControllableFeedbackOnly(true);
    colorManager.length->isSavable = false;

    addChildControllableContainer(&colorManager);

    setupMappingInputParameter(colorManager.currentColor);

    uiHeight->setValue(60);
}

ColorMappingLayer::~ColorMappingLayer()
{
}

void ColorMappingLayer::addDefaultContent()
{
    colorManager.addColorAt(0, Colours::red);
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

Array<Inspectable*> ColorMappingLayer::selectAllItemsBetweenInternal(float start, float end)
{
    Array<Inspectable*> result;
    result.addArray(colorManager.getItemsInTimespan(start, end));
    return result;
}

Array<UndoableAction*> ColorMappingLayer::getRemoveAllItemsBetweenInternal(float start, float end)
{
    return colorManager.getRemoveItemsUndoableAction(colorManager.getItemsInTimespan(start, end));
}

Array<UndoableAction*> ColorMappingLayer::getInsertTimespanInternal(float start, float length)
{
    return colorManager.getMoveKeysBy(start, length);
}

Array<UndoableAction*> ColorMappingLayer::getRemoveTimespanInternal(float start, float end)
{
    return colorManager.getRemoveTimespan(start, end);
}

bool ColorMappingLayer::paste()
{
    var data = JSON::fromString(SystemClipboard::getTextFromClipboard());
    String type = data.getProperty("itemType", "");
    if (type == colorManager.itemDataType)
    {
        colorManager.askForPaste();
        return true;
    }

    return SequenceLayer::paste();
}

void ColorMappingLayer::getSnapTimes(Array<float>* arrayToFill)
{
    for (auto& i : colorManager.items)
    {
        arrayToFill->addIfNotAlreadyThere(i->position->floatValue());
    }
}

void ColorMappingLayer::getSequenceSnapTimesForManager(Array<float>* arrayToFill)
{
    sequence->getSnapTimes(arrayToFill);
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
