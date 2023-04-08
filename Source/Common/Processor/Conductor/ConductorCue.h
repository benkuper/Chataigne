/*
  ==============================================================================

    ConductorCue.h
    Created: 5 Oct 2021 9:40:49pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConductorCue :
    public Action,
    public Sequence::SequenceListener
{
public:
    ConductorCue(var params = var(), Multiplex * multliplex = nullptr);
    ~ConductorCue();

    bool isCurrent;
    bool isNext;
    int index;

    ControllableContainer seqCC;
    TargetParameter* linkedSequenceParam;
    Sequence* linkedSequence;
    BoolParameter* autoStart;
    BoolParameter* forceStartFrom0;
    BoolParameter* autoStop;
    BoolParameter* autoNext;

    void clearItem() override;

    void setLinkedSequence(Sequence* s);

    void setIsCurrent(bool value);
    void setIsNext(bool value);
    void setIndex(int value);

    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

    void sequenceFinished(Sequence* s) override;

    ProcessorUI* getUI() override;

    virtual var getJSONData() override;
    virtual void loadJSONDataItemInternal(var data) override;
    
    String getTypeString() const override { return "Cue"; }

    DECLARE_ASYNC_EVENT(ConductorCue, ConductorCue, conductorCue, ENUM_LIST(CURRENT_CHANGED, NEXT_CHANGED, INDEX_CHANGED));
};