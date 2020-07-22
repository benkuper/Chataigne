/*
  ==============================================================================

    GPIOModule.cpp
    Created: 21 Jul 2020 2:56:50pm
    Author:  bkupe

  ==============================================================================
*/

#include "GPIOModule.h"

GPIOModule::GPIOModule(var params) :
    Module("GPIO"),
    gpioModes("GPIO Modes")
{
    for (int i = 0; i < GPIO_MAX_PINS; i++)
    {
        EnumParameter * p = 
    }
}

GPIOModule::~GPIOModule()
{
}
