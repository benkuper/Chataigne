/*
  ==============================================================================

    ChataigneGPIO.c
    Created: 28 Aug 2026

    See ChataigneGPIO.h for the rationale. Implementation notes:

    - Each GPIO line is requested from the kernel lazily, the first time
      gpioWrite/gpioPWM (as an output) or gpioRead (as an input) is called
      for that pin, and reconfigured on the fly if the direction changes.
      GPIOModule never called pigpio's gpioSetMode either, so this mirrors
      the behaviour the module already relied on.
    - gpioPWM has no hardware PWM to fall back to for arbitrary header pins,
      so, like pigpio itself for non hardware-PWM-capable GPIOs, it runs a
      small software PWM thread per pin that toggles the line with the
      requested duty cycle.
    - All access to the chip/line-request state is serialized with a single
      mutex: GPIOModule reads and writes GPIOs from two different threads
      (its own polling thread for reads, whatever thread triggers a command
      for writes).

  ==============================================================================
*/

#if USE_GPIO

#include "ChataigneGPIO.h"

#include <gpiod.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

typedef struct
{
    struct gpiod_line* line;
    int direction;

    pthread_t pwmThread;
    volatile int pwmRunning;
    volatile unsigned pwmDuty; // 0-255
} PinState;

static struct gpiod_chip* gpioChip = NULL;
static PinState gpioPins[GPIO_MAX_USER_PIN + 1];
static pthread_mutex_t gpioMutex = PTHREAD_MUTEX_INITIALIZER;

static struct gpiod_chip* openMainChip(void)
{
    char path[32];

    for (int i = 0; i < 8; i++)
    {
        snprintf(path, sizeof(path), "/dev/gpiochip%d", i);
        struct gpiod_chip* c = gpiod_chip_open(path);
        if (c == NULL) continue;

        const char* label = gpiod_chip_label(c);
        int isMainChip = (label != NULL) && (strncmp(label, "pinctrl", 7) == 0);

        if (isMainChip) return c;
        gpiod_chip_close(c);
    }

    // Fallback for kernels/boards where the main controller isn't labelled "pinctrl-*".
    return gpiod_chip_open("/dev/gpiochip0");
}

// Must be called with gpioMutex held. Requests pin's line for the given direction.
static struct gpiod_line* ensureLine(unsigned pin, int direction, int initialValue)
{
    PinState* ps = &gpioPins[pin];

    if (ps->line != NULL && ps->direction == direction) return ps->line;

    if (ps->line != NULL)
    {
        gpiod_line_release(ps->line);
        ps->line = NULL;
    }

    struct gpiod_line* line = gpiod_chip_get_line(gpioChip, pin);
    if (line == NULL) return NULL;

    int result = direction == GPIOD_LINE_DIRECTION_OUTPUT
        ? gpiod_line_request_output(line, "Chataigne", initialValue ? 1 : 0)
        : gpiod_line_request_input(line, "Chataigne");

    if (result < 0) return NULL;

    ps->line = line;
    ps->direction = direction;
    return ps->line;
}

// Sets a pin's output value without touching any running software-PWM thread for that pin.
static int rawWrite(unsigned pin, unsigned value)
{
    pthread_mutex_lock(&gpioMutex);

    struct gpiod_line* line = ensureLine(pin, GPIOD_LINE_DIRECTION_OUTPUT, value ? 1 : 0);
    int result = (line != NULL) ? gpiod_line_set_value(line, value ? 1 : 0) : -1;

    pthread_mutex_unlock(&gpioMutex);
    return result;
}

static void stopPwm(unsigned pin)
{
    pthread_t threadToJoin;
    int shouldJoin = 0;

    pthread_mutex_lock(&gpioMutex);
    if (gpioPins[pin].pwmRunning)
    {
        gpioPins[pin].pwmRunning = 0;
        threadToJoin = gpioPins[pin].pwmThread;
        shouldJoin = 1;
    }
    pthread_mutex_unlock(&gpioMutex);

    if (shouldJoin) pthread_join(threadToJoin, NULL);
}

static void* pwmThreadFunc(void* arg)
{
    unsigned pin = (unsigned)(uintptr_t)arg;
    const long periodNs = 4 * 1000 * 1000; // ~250Hz, plenty for LEDs/dimmers driven from software

    for (;;)
    {
        pthread_mutex_lock(&gpioMutex);
        int running = gpioPins[pin].pwmRunning;
        unsigned duty = gpioPins[pin].pwmDuty;
        pthread_mutex_unlock(&gpioMutex);

        if (!running) break;

        long onNs = (periodNs * (long)duty) / 255;
        long offNs = periodNs - onNs;

        struct timespec ts;
        if (onNs > 0)
        {
            rawWrite(pin, 1);
            ts.tv_sec = onNs / (1000 * 1000 * 1000);
            ts.tv_nsec = onNs % (1000 * 1000 * 1000);
            nanosleep(&ts, NULL);
        }

        if (offNs > 0)
        {
            rawWrite(pin, 0);
            ts.tv_sec = offNs / (1000 * 1000 * 1000);
            ts.tv_nsec = offNs % (1000 * 1000 * 1000);
            nanosleep(&ts, NULL);
        }
    }

    return NULL;
}

int gpioInitialise(void)
{
    pthread_mutex_lock(&gpioMutex);

    if (gpioChip != NULL)
    {
        pthread_mutex_unlock(&gpioMutex);
        return 0;
    }

    gpioChip = openMainChip();
    if (gpioChip == NULL)
    {
        pthread_mutex_unlock(&gpioMutex);
        return -1;
    }

    memset(gpioPins, 0, sizeof(gpioPins));
    pthread_mutex_unlock(&gpioMutex);
    return 0;
}

void gpioTerminate(void)
{
    for (unsigned pin = 0; pin <= GPIO_MAX_USER_PIN; pin++) stopPwm(pin);

    pthread_mutex_lock(&gpioMutex);
    for (unsigned pin = 0; pin <= GPIO_MAX_USER_PIN; pin++)
    {
        if (gpioPins[pin].line != NULL)
        {
            gpiod_line_release(gpioPins[pin].line);
            gpioPins[pin].line = NULL;
        }
    }

    if (gpioChip != NULL)
    {
        gpiod_chip_close(gpioChip);
        gpioChip = NULL;
    }
    pthread_mutex_unlock(&gpioMutex);
}

int gpioWrite(unsigned pin, unsigned value)
{
    if (pin > GPIO_MAX_USER_PIN || gpioChip == NULL) return -1;
    stopPwm(pin);
    return rawWrite(pin, value);
}

int gpioRead(unsigned pin)
{
    if (pin > GPIO_MAX_USER_PIN || gpioChip == NULL) return -1;

    pthread_mutex_lock(&gpioMutex);
    struct gpiod_line* line = ensureLine(pin, GPIOD_LINE_DIRECTION_INPUT, 0);
    int result = (line != NULL) ? gpiod_line_get_value(line) : -1;
    pthread_mutex_unlock(&gpioMutex);

    return result;
}

int gpioPWM(unsigned pin, unsigned value)
{
    if (pin > GPIO_MAX_USER_PIN || gpioChip == NULL) return -1;

    if (value == 0)
    {
        stopPwm(pin);
        return rawWrite(pin, 0);
    }

    if (value >= 255)
    {
        stopPwm(pin);
        return rawWrite(pin, 1);
    }

    pthread_mutex_lock(&gpioMutex);
    gpioPins[pin].pwmDuty = value;
    int alreadyRunning = gpioPins[pin].pwmRunning;
    if (!alreadyRunning) gpioPins[pin].pwmRunning = 1;
    pthread_mutex_unlock(&gpioMutex);

    if (!alreadyRunning)
    {
        if (pthread_create(&gpioPins[pin].pwmThread, NULL, pwmThreadFunc, (void*)(uintptr_t)pin) != 0)
        {
            pthread_mutex_lock(&gpioMutex);
            gpioPins[pin].pwmRunning = 0;
            pthread_mutex_unlock(&gpioMutex);
            return -1;
        }
    }

    return 0;
}

#endif // USE_GPIO
