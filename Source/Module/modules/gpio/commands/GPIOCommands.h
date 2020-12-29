/*
  ==============================================================================

    GPIOCommands.h
    Created: 21 Jul 2020 2:57:07pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "Common/Command/BaseCommand.h"

class GPIOModule;

class GPIOCommand :
    public BaseCommand
{
public:
    GPIOCommand(GPIOModule* m, CommandContext context, var params, Multiplex* multiplex = nullptr);
    ~GPIOCommand();

    GPIOModule* gpioModule;

    enum GPIOAction { SET_DIGITAL, SET_PWM };
    GPIOAction action;

    IntParameter* pin;
    Parameter* valueParam;

    void triggerInternal(int multiplexIndex) override;
    
    static GPIOCommand* create(ControllableContainer * cc, CommandContext context, var params, Multiplex * multiplex = nullptr) { return new GPIOCommand((GPIOModule *)cc, context, params, multiplex); }
};