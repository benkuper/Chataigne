/*
  ==============================================================================

    AutomationMappingLayer.cpp
    Created: 25 Mar 2020 1:00:31pm
    Author:  bkupe

  ==============================================================================
*/


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
    //automation->hideInEditor = true;
    automation->setLength(sequence->totalTime->floatValue(), true);
    automation->length->setControllableFeedbackOnly(true); //force not saving and not changing from user
    automation->length->isSavable = false;
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

Array<Inspectable*> AutomationMappingLayer::selectAllItemsBetweenInternal(float start, float end)
{
    Array<Inspectable*> result;
    Array<AutomationKey*> keys = automation->getKeysBetweenPositions(start, end);
    result.addArray(keys);
    return result;
}

Array<UndoableAction*> AutomationMappingLayer::getRemoveAllItemsBetweenInternal(float start, float end)
{
    return automation->getRemoveItemsUndoableAction(automation->getKeysBetweenPositions(start, end));
}

Array<UndoableAction*> AutomationMappingLayer::getInsertTimespanInternal(float start, float length)
{
    return automation->getMoveKeysBy(start, length);
}

Array<UndoableAction*> AutomationMappingLayer::getRemoveTimespanInternal(float start, float end)
{
    return automation->getRemoveTimespan(start, end);
}

void AutomationMappingLayer::getSnapTimes(Array<float>* arrayToFill)
{
    if (automation == nullptr) return;
    for (auto& i : automation->items)
    {
        arrayToFill->addIfNotAlreadyThere(i->position->floatValue());
    }
}

void AutomationMappingLayer::getSequenceSnapTimesForAutomation(Array<float>* arrayToFill, AutomationKey * k)
{
    sequence->getSnapTimes(arrayToFill);
    int index = automation->items.indexOf(k);
    AutomationKey* k1 = index > 0 ? automation->items[index - 1] : nullptr;
    AutomationKey* k2 = index < automation->items.size() - 1 ? automation->items[index + 1] : nullptr;

    float t1 = k1 != nullptr ? k1->position->floatValue() : 0;
    float t2 = k2 != nullptr ? k2->position->floatValue() : sequence->totalTime->floatValue();

    Array<float> keysToRemove;
    for (auto& t : *arrayToFill)
    {
        if (t <= t1 || t >= t2) keysToRemove.add(t);
    }

    for (auto& t : keysToRemove) arrayToFill->removeAllInstancesOf(t);
}


void AutomationMappingLayer::sequenceCurrentTimeChangedInternal(Sequence* s, float prevTime, bool seeking)
{
    if (automation == nullptr) return;

    float curTime = sequence->currentTime->floatValue();
    automation->position->setValue(curTime);

    if (sequence->isPlaying->boolValue())
    {
        if (recorder.isRecording->boolValue())
        {
            if (sequence->currentTime->floatValue() < prevTime)  recorder.removeKeysAfter(curTime);
            recorder.addKeyAt(curTime);
        }
    }
}

void AutomationMappingLayer::sequenceTotalTimeChanged(Sequence* s)
{
    automation->setLength(s->totalTime->floatValue());
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
    if(recorder.isRecording->boolValue()) stopRecorderAndAddKeys();
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

