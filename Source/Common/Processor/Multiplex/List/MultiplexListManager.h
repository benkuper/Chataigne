/*
  ==============================================================================

    MultiplexListManager.h
    Created: 19 Dec 2020 12:00:43pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class Multiplex;

class MultiplexListManager :
    public Manager<BaseMultiplexList>
{
public:
    MultiplexListManager(Multiplex* multiplex);
    ~MultiplexListManager();

    Multiplex * multiplex;

    void addItemInternal(BaseMultiplexList * item, var data) override;

    Factory<BaseMultiplexList> factory;
};