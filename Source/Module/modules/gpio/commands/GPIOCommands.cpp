/*
  ==============================================================================

    GPIOCommands.cpp
    Created: 21 Jul 2020 2:57:07pm
    Author:  bkupe

  ==============================================================================
*/

GPIOCommand::GPIOCommand(GPIOModule* m, CommandContext context, var params, Multiplex* multiplex) :
    BaseCommand(m, context, params, multiplex),
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
        linkParamToMappingIndex(valueParam, 0);
    }
}

GPIOCommand::~GPIOCommand()
{
}

void GPIOCommand::triggerInternal(int multiplexIndex)
{
    switch (action)
    {
    case SET_DIGITAL:
        gpioModule->setDigitalValue(getLinkedValue(pin, multiplexIndex), getLinkedValue(valueParam, multiplexIndex));
        break;

    case SET_PWM:
        gpioModule->setPWMValue(getLinkedValue(pin, multiplexIndex), getLinkedValue(valueParam, multiplexIndex));
        break;
    }
}
