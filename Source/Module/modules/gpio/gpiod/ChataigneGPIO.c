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
    struct gpiod_line_request* request;
    enum gpiod_line_direction direction;

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

        struct gpiod_chip_info* info = gpiod_chip_get_info(c);
        int isMainChip = 0;
        if (info != NULL)
        {
            const char* label = gpiod_chip_info_get_label(info);
            isMainChip = (label != NULL) && (strncmp(label, "pinctrl", 7) == 0);
            gpiod_chip_info_free(info);
        }

        if (isMainChip) return c;
        gpiod_chip_close(c);
    }

    // Fallback for kernels/boards where the main controller isn't labelled "pinctrl-*".
    return gpiod_chip_open("/dev/gpiochip0");
}

// Must be called with gpioMutex held. Requests (or reconfigures) pin's line for the given direction.
static struct gpiod_line_request* ensureLine(unsigned pin, enum gpiod_line_direction direction, enum gpiod_line_value initialValue)
{
    PinState* ps = &gpioPins[pin];

    if (ps->request != NULL && ps->direction == direction) return ps->request;

    struct gpiod_line_settings* settings = gpiod_line_settings_new();
    gpiod_line_settings_set_direction(settings, direction);
    if (direction == GPIOD_LINE_DIRECTION_OUTPUT) gpiod_line_settings_set_output_value(settings, initialValue);

    struct gpiod_line_config* lineConfig = gpiod_line_config_new();
    unsigned int offset = pin;
    gpiod_line_config_add_line_settings(lineConfig, &offset, 1, settings);

    if (ps->request != NULL)
    {
        if (gpiod_line_request_reconfigure_lines(ps->request, lineConfig) == 0)
        {
            ps->direction = direction;
        }
        else
        {
            gpiod_line_request_release(ps->request);
            ps->request = NULL;
        }
    }

    if (ps->request == NULL)
    {
        struct gpiod_request_config* requestConfig = gpiod_request_config_new();
        gpiod_request_config_set_consumer(requestConfig, "Chataigne");

        ps->request = gpiod_chip_request_lines(gpioChip, requestConfig, lineConfig);
        if (ps->request != NULL) ps->direction = direction;

        gpiod_request_config_free(requestConfig);
    }

    gpiod_line_config_free(lineConfig);
    gpiod_line_settings_free(settings);

    return ps->request;
}

// Sets a pin's output value without touching any running software-PWM thread for that pin.
static int rawWrite(unsigned pin, unsigned value)
{
    pthread_mutex_lock(&gpioMutex);

    enum gpiod_line_value v = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    struct gpiod_line_request* req = ensureLine(pin, GPIOD_LINE_DIRECTION_OUTPUT, v);
    int result = (req != NULL) ? gpiod_line_request_set_value(req, pin, v) : -1;

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
    gpioChip = openMainChip();
    if (gpioChip == NULL) return -1;

    memset(gpioPins, 0, sizeof(gpioPins));
    return 0;
}

void gpioTerminate(void)
{
    for (unsigned pin = 0; pin <= GPIO_MAX_USER_PIN; pin++) stopPwm(pin);

    pthread_mutex_lock(&gpioMutex);
    for (unsigned pin = 0; pin <= GPIO_MAX_USER_PIN; pin++)
    {
        if (gpioPins[pin].request != NULL)
        {
            gpiod_line_request_release(gpioPins[pin].request);
            gpioPins[pin].request = NULL;
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
    struct gpiod_line_request* req = ensureLine(pin, GPIOD_LINE_DIRECTION_INPUT, GPIOD_LINE_VALUE_INACTIVE);
    int result = -1;
    if (req != NULL)
    {
        enum gpiod_line_value v = gpiod_line_request_get_value(req, pin);
        result = (v == GPIOD_LINE_VALUE_ACTIVE) ? 1 : (v == GPIOD_LINE_VALUE_INACTIVE ? 0 : -1);
    }
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

    if (!alreadyRunning) pthread_create(&gpioPins[pin].pwmThread, NULL, pwmThreadFunc, (void*)(uintptr_t)pin);

    return 0;
}

#endif // USE_GPIO
