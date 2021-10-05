/*
  ==============================================================================

    ConductorCue.h
    Created: 5 Oct 2021 9:40:49pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class ConductorCue :
    public Action
{
public:
    ConductorCue(var params = var(), Multiplex * multliplex = nullptr);
    ~ConductorCue();

    bool isCurrent;

    void setIsCurrent(bool value);

    ProcessorUI* getUI() override;
    
    String getTypeString() const override { return "Cue"; }

    DECLARE_ASYNC_EVENT(ConductorCue, ConductorCue, conductorCue, { CURRENT_CHANGED });
};