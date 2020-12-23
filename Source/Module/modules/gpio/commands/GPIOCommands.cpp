/*
  ==============================================================================

    GPIOCommands.cpp
    Created: 21 Jul 2020 2:57:07pm
    Author:  bkupe

  ==============================================================================
*/

#include "GPIOCommands.h"
#include "../GPIOModule.h"

GPIOCommand::GPIOCommand(GPIOModule* m, CommandContext context, var params, IteratorProcessor* iterator) :
    BaseCommand(m, context, params, iterator),
    gpioModule(m),
    valueParam(nullptr)
{

    action = (GPIOAction)(int)params.getProperty("action", GPIOAction::SET_DIGITAL);

    pin = addIntParameter("GPIO Number", "Number of the GPIO to set", 0, 0,  GPIO_MAX_PINS);

    switch (action)
    {
    case SET_DIGITAL:
        valueParam = addBoolParameter("Value", "Value of this pin. Since it's digital, it can only be HIGH or LOW", false);
        break;

    case SET_PWM:
        valueParam = addFloatParameter("Value", "Value of this pin. Since it's analog, this is the normalized value", 0, 0, 1);
        break;
    }

    if (valueParam != nullptr)
    {
        //addTargetMappingParameterAt(valueParam, 0);
    }
}

GPIOCommand::~GPIOCommand()
{
}

void GPIOCommand::triggerInternal()
{
    switch (action)
    {
    case SET_DIGITAL:
        gpioModule->setDigitalValue(pin->intValue(), valueParam->boolValue());
        break;

    case SET_PWM:
        gpioModule->setPWMValue(pin->intValue(), valueParam->floatValue());
        break;
    }
}
