/*
  ==============================================================================

    GPIOModule.h
    Created: 21 Jul 2020 2:56:50pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Module.h"

//tmp
#define __arm__

#ifdef __arm__
#define GPIO_SUPPORT
#include "pigpio/pigpio.h"
#define GPIO_MAX_PINS PI_MAX_USER_GPIO
#else
#define GPIO_MAX_PINS 32
#endif

class GPIOModule :
    public Module
{
public:
    GPIOModule(var params);
    virtual ~GPIOModule();

    ControllableContainer gpioModes;
    Array<EnumParameter*> gpioModeParams;

    Array<Parameter*> gpioInputParams;

    String getDefaultTypeString() { return "GPIO"; }

    static GPIOModule* create(var params) { return new GPIOModule(params); }
};