/*
  ==============================================================================

    AutomationMappingLayer.cpp
    Created: 25 Mar 2020 1:00:31pm
    Author:  bkupe

  ==============================================================================
*/

#include "AutomationMappingLayer.h"
#include "ui/AutomationMappingLayerPanel.h"
#include "ui/AutomationMappingLayerTimeline.h"

AutomationMappingLayer::AutomationMappingLayer(const String& name, Sequence* s, var params) :
    MappingLayer(name, s, params),
    automation(nullptr)
{
    recordSendMode = addEnumParameter("Record Send Mode", "Choose what to do when recording");
    recordSendMode->addOption("Do not send", DONOTSEND)->addOption("Send original value", SEND_ORIGINAL)->addOption("Send new value", SEND_NEW);

    recorder.input->customGetTargetFunc = &ModuleManager::showAllValuesAndGetControllable;
    recorder.input->customGetControllableLabelFunc = &Module::getTargetLabelForValueControllable;
    recorder.input->customCheckAssignOnNextChangeFunc = &ModuleManager::checkControllableIsAValue;
    recorder.editorIsCollapsed = true;
    addChildControllableContainer(&recorder,false, 0);

    uiHeight->setValue(120);
}

AutomationMappingLayer::~AutomationMappingLayer()
{
}

void AutomationMappingLayer::setupAutomation(Automation* a)
{
    jassert(automation == nullptr);
    automation = a;
    automation->hideInEditor = true;
    automation->setLength(sequence->totalTime->floatValue(), true);
}

void AutomationMappingLayer::updateMappingInputValueInternal()
{
    if (!recorder.isRecording->boolValue())
    {
        MappingLayer::updateMappingInputValueInternal();
    }
    else
    {
        RecordSendMode m = recordSendMode->getValueDataAsEnum<RecordSendMode>();
        if (m == SEND_ORIGINAL)
        {
            MappingLayer::updateMappingInputValueInternal();
        }
        else if (m == SEND_NEW)
        {
            if (recorder.keys.size() > 0) mappingInput->setValue(recorder.keys[recorder.keys.size() - 1].value);
        }
    }

}

void AutomationMappingLayer::selectAll(bool addToSelection)
{
    deselectThis(automation->items.size() == 0);
    automation->askForSelectAllItems(addToSelection);
}

void AutomationMappingLayer::sequenceCurrentTimeChangedInternal(Sequence* s, float prevTime, bool seeking)
{
    automation->position->setValue(s->currentTime->floatValue());

    if (sequence->isPlaying->boolValue())
    {
        if (recorder.isRecording->boolValue())
        {
            if (prevTime < sequence->currentTime->floatValue()) recorder.addKeyAt(sequence->currentTime->floatValue());
            else  recorder.startRecording();
        }
    }
}

void AutomationMappingLayer::sequenceTotalTimeChanged(Sequence* s)
{
    automation->length->setValue(s->totalTime->floatValue());
}

void AutomationMappingLayer::sequencePlayStateChangedInternal(Sequence* s)
{
    if (sequence->isPlaying->boolValue())
    {
        if (recorder.shouldRecord()) recorder.startRecording();
    }
    else if (recorder.isRecording->boolValue())
    {
        stopRecorderAndAddKeys();
    }
}

void AutomationMappingLayer::sequenceLooped(Sequence* s)
{
    stopRecorderAndAddKeys();
}

bool AutomationMappingLayer::paste()
{
    var data = JSON::fromString(SystemClipboard::getTextFromClipboard());
    String type = data.getProperty("itemType", "");
    if (type == automation->itemDataType)
    {
        automation->askForPaste();
        return true;
    }

    return SequenceLayer::paste();
}


SequenceLayerPanel* AutomationMappingLayer::getPanel()
{
    return new AutomationMappingLayerPanel(this);
}

SequenceLayerTimeline* AutomationMappingLayer::getTimelineUI()
{
    return new AutomationMappingLayerTimeline(this);
}

