/*
  ==============================================================================

    GPIOModule.cpp
    Created: 21 Jul 2020 2:56:50pm
    Author:  bkupe

  ==============================================================================
*/

GPIOModule::GPIOModule() :
    Module("GPIO"),
    Thread("GPIO"),
    gpioModes("GPIO Modes")
{
    setupIOConfiguration(true, true);

    for (int i = 0; i < GPIO_MAX_PINS; i++)
    {
        EnumParameter* p = gpioModes.addEnumParameter("GPIO "+String(i)+" Mode", "Mode for the GPIO on pin "+String(i));
        p->addOption("Output", GPIOMode::OUTPUT)->addOption("Input", GPIOMode::INPUT)
         ->addOption("Alt 0", GPIOMode::ALT0)->addOption("Alt 1", GPIOMode::ALT1)->addOption("Alt 2", GPIOMode::ALT2)
         ->addOption("Alt 3", GPIOMode::ALT3)->addOption("Alt 4", GPIOMode::ALT4)-> addOption("Alt 5", GPIOMode::ALT5);

        gpioModeParams.add(p);

        FloatParameter* fp = valuesCC.addFloatParameter("GPIO " + String(i), "Value for GPIO " + String(i), 0, 0, 1);
        gpioInputParams.add(fp);
    }

    moduleParams.addChildControllableContainer(&gpioModes);

    defManager->add(CommandDefinition::createDef(this, "", "Set Digital", &GPIOCommand::create, CommandContext::BOTH)->addParam("action", GPIOCommand::SET_DIGITAL));
    defManager->add(CommandDefinition::createDef(this, "", "Set PWM", &GPIOCommand::create, CommandContext::BOTH)->addParam("action", GPIOCommand::SET_PWM));
    
#ifdef GPIO_SUPPORT
    if (gpioInitialise() < 0)
    {
        LOGERROR("Error initializing GPIO, do you have the rights ?\nTry launching Chataigne with sudo or settings the udev rules for GPIO :\nhttps://roboticsbackend.com/raspberry-pi-hardware-permissions/#GPIOs");
    }
    else
    {
        LOG("GPIO is initialized");
    }
#endif

    startThread();
}

GPIOModule::~GPIOModule()
{
    stopThread(1000);

#ifdef GPIO_SUPPORT
    gpioTerminate();
#endif
}

void GPIOModule::setDigitalValue(int pin, bool value)
{
    if (!enabled->boolValue()) return;

#ifdef GPIO_SUPPORT
    gpioWrite(pin, (int)value);
#endif

    outActivityTrigger->trigger();
    if (logOutgoingData->boolValue()) NLOG(niceName, "Set pin " << pin << " to " << (value ? "HIGH" : "LOW"));
}

void GPIOModule::setPWMValue(int pin, float value)
{
    if (!enabled->boolValue()) return;

    int finalValue = (int)(value * 255);
#ifdef GPIO_SUPPORT
    gpioPWM(pin, finalValue);
#endif

    outActivityTrigger->trigger();
    if (logOutgoingData->boolValue()) NLOG(niceName, "Set pin " << pin << " to " << finalValue);
}

void GPIOModule::onContainerParameterChanged(Parameter* p)
{
    Module::onContainerParameterChanged(p);

    if (p == enabled)
    {
        if (enabled->boolValue()) startThread();
        else stopThread(1000);
    }
}

void GPIOModule::run()
{
#ifdef GPIO_SUPPORT
    while (!threadShouldExit())
    {
        //read
        for (int i = 0; i < GPIO_MAX_PINS; i++)
        {
            bool isInput = gpioModeParams[i]->getValueDataAsEnum<GPIOMode>() == GPIOMode::INPUT;
            if (!isInput) continue;
            int value = gpioRead(i);

            if (value != gpioInputParams[i]->intValue())
            {
                inActivityTrigger->trigger();
                if (logIncomingData->boolValue()) NLOG(niceName, "GPIO " << i << " updated : " << value);
                gpioInputParams[i]->setValue(value);
            }
        }

        wait(10); //around 100fps
    }
#endif
}
