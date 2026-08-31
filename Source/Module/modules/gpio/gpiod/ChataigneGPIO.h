/*
  ==============================================================================

    ChataigneGPIO.h
    Created: 28 Aug 2026

    Minimal GPIO backend for Raspberry Pi boards, built on top of libgpiod
    (the Linux GPIO character-device API, https://libgpiod.readthedocs.io).

    This replaces the previously vendored pigpio library. pigpio talks to
    the GPIO controller by mapping its registers directly from /dev/mem,
    which only works for the BCM283x/BCM2711 SoCs used up to the Raspberry
    Pi 4. The Raspberry Pi 5 moved GPIO handling to a separate RP1 chip with
    a different register layout that pigpio (unmaintained since 2021) never
    learned to talk to, so on a Pi 5 gpioInitialise() always fails.

    libgpiod instead goes through the kernel's gpiochip character device,
    which is implemented for every Pi model (RP1 included) and is the
    approach the kernel/Raspberry Pi foundation now recommend. It also means
    Chataigne no longer needs root or a running pigpiod daemon: normal
    membership of the "gpio" group is enough.

    Only the small subset of the pigpio API actually used by GPIOModule is
    reproduced here, with the same names/signatures, so the rest of the
    module needed no changes.

  ==============================================================================
*/

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Highest user GPIO index exposed by Chataigne's UI (same value pigpio used for PI_MAX_USER_GPIO).
#define GPIO_MAX_USER_PIN 31

// Returns 0 on success, -1 on failure (matches pigpio's gpioInitialise contract of "< 0 is an error").
int gpioInitialise(void);
void gpioTerminate(void);

// value is 0 or 1. Requests the pin as an output the first time it is called for a given pin.
int gpioWrite(unsigned pin, unsigned value);

// Requests the pin as an input the first time it is called for a given pin. Returns 0, 1, or -1 on error.
int gpioRead(unsigned pin);

// value is 0-255. There is no hardware PWM on the RP1 header pins exposed here, so this drives
// a lightweight software PWM thread, same as pigpio does for GPIOs without hardware PWM.
int gpioPWM(unsigned pin, unsigned value);

#ifdef __cplusplus
}
#endif
