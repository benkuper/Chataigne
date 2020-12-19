/*
  ==============================================================================

    Iterator.h
    Created: 17 Dec 2020 5:12:36pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../ProcessorManager.h"
#include "List/IteratorListManager.h"

class IteratorProcessor :
    public Processor
{
public:
    IteratorProcessor(var params = var());
    ~IteratorProcessor();

    IntParameter* count;
    IteratorListManager listManager;
    ProcessorManager processorManager;

    void onContainerParameterChangedInternal(Parameter* p) override;

    void updateDisables(bool force) override;

    BaseIteratorList* showAndGetList();

    ProcessorUI* getUI() override;
    String getTypeString() const override { return "Iterator"; };
};