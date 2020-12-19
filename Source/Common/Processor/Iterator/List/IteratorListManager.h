/*
  ==============================================================================

    IteratorListManager.h
    Created: 19 Dec 2020 12:00:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "IteratorList.h"

class IteratorProcessor;

class IteratorListManager :
    public BaseManager<BaseIteratorList>
{
public:
    IteratorListManager(IteratorProcessor* iterator);
    ~IteratorListManager();

    IteratorProcessor * iterator;

    void addItemInternal(BaseIteratorList * item, var data) override;

    Factory<BaseIteratorList> factory;
};