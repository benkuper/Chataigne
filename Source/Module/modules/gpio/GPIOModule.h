/*
  ==============================================================================

	GPIOModule.h
	Created: 21 Jul 2020 2:56:50pm
	Author:  bkupe

  ==============================================================================
*/

#pragma once

//tmp
//#define __arm__

#ifndef GPIO_SUPPPORT
#if JUCE_LINUX
#if defined(__arm__) || defined(__aarch64__)
#define GPIO_SUPPORT
#endif
#endif
#endif


#ifdef GPIO_SUPPORT
#include "pigpio/pigpio.h"
#define GPIO_MAX_PINS PI_MAX_USER_GPIO
#else
#define GPIO_MAX_PINS 32
#endif

class GPIOModule :
	public Module,
	public Thread
{
public:
	GPIOModule();
	virtual ~GPIOModule();

	enum GPIOMode { OUTPUT = 0, INPUT = 1, ALT0 = 4, ALT1 = 5, ALT2 = 6, ALT3 = 7, ALT4 = 3, ALT5 = 2, GPIO_MODE_MAX = 8 };


	ControllableContainer gpioModes;
	Array<EnumParameter*> gpioModeParams;

	Array<Parameter*> gpioInputParams;

	void setDigitalValue(int pin, bool value);
	void setPWMValue(int pin, float value);

	void onContainerParameterChanged(Parameter* p) override;

	void run() override;

	String getDefaultTypeString() const override { return "GPIO"; }

	static GPIOModule* create() { return new GPIOModule(); }
};