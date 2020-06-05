/*
  ==============================================================================

    AutomationMappingLayer.h
    Created: 25 Mar 2020 1:00:31pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../MappingLayer.h"

class AutomationMappingLayer :
    public MappingLayer
{
public:
    AutomationMappingLayer(const String &name, Sequence* s, var params);
    ~AutomationMappingLayer();

    Automation* automation;

    enum RecordSendMode { DONOTSEND, SEND_ORIGINAL, SEND_NEW };
    EnumParameter* recordSendMode;
    AutomationRecorder recorder;

   
    virtual void setupAutomation(Automation* a);

    virtual void updateMappingInputValueInternal() override;
    virtual void stopRecorderAndAddKeys() {}

    void selectAll(bool addToSelection = false) override;

    Array<Inspectable*> selectAllItemsBetweenInternal(float start, float end) override;
    Array<UndoableAction*> getRemoveAllItemsBetweenInternal(float start, float end) override;
    Array<UndoableAction*>  getInsertTimespanInternal(float start, float length) override;
    Array<UndoableAction*>  getRemoveTimespanInternal(float start, float end) override;


    virtual void sequenceCurrentTimeChangedInternal(Sequence* s, float prevTime, bool seeking) override;
    virtual void sequenceTotalTimeChanged(Sequence* s) override;
    virtual void sequencePlayStateChangedInternal(Sequence* s) override;
    virtual void sequenceLooped(Sequence *s) override;

    virtual bool paste() override;

    SequenceLayerPanel* getPanel() override;
    SequenceLayerTimeline* getTimelineUI() override;
};

