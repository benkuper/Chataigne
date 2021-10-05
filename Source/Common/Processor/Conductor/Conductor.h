/*
  ==============================================================================

    Conductor.h
    Created: 5 Oct 2021 9:35:28pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Conductor :
    public Action,
    public ProcessorManager::ManagerListener
{
public:
    Conductor(var params = var(), Multiplex * multiplex = nullptr);
    ~Conductor();

    IntParameter* cueIndex;
    BoolParameter* loop;

    ProcessorManager processorManager;

    void onContainerParameterChangedInternal(Parameter* p) override;
    void updateDisables(bool force) override;

    void itemAdded(Processor* p) override;
    void itemsAdded(Array<Processor*> p) override;
    void itemRemoved(Processor* p) override;
    void itemsRemoved(Array<Processor *> p) override;

    void triggerConsequences(bool triggerTrue, int multiplexIndex = 0) override;

    int getNextValidIndex();
    int getFirstValidIndex();

    void updateCurrentCue();

    void afterLoadJSONDataInternal() override;

    ProcessorUI* getUI() override;
    String getTypeString() const override { return "Conductor"; };
};
