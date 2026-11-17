// Fake Phidget22 ABI for deterministic integration tests, without USB hardware.
#include <windows.h>
#include <array>

#define API extern "C" __declspec(dllexport) int __stdcall
struct Channel { int type, index = 0, serial = -1; bool open = false; };
static bool attached = true;
static int alive = 0, writeError = 0, openError = 0;
static std::array<std::array<double, 64>, 5> values {};
API Mock_setAttached(int value) { attached = value != 0; return 0; }
API Mock_alive() { return alive; }
API Mock_setWriteError(int value) { writeError = value; return 0; }
API Mock_setOpenError(int value) { openError = value; return 0; }
API Mock_setValue(int type, int index, double value) { values[type][index] = value; return 0; }
API Mock_reset() { attached = true; writeError = openError = 0; values = {}; return 0; }

static int create(void** handle, int type) { *handle = new Channel { type }; ++alive; return 0; }
API PhidgetDigitalInput_create(void** h) { return create(h, 0); }
API PhidgetDigitalOutput_create(void** h) { return create(h, 1); }
API PhidgetVoltageInput_create(void** h) { return create(h, 2); }
API PhidgetVoltageRatioInput_create(void** h) { return create(h, 3); }
API PhidgetVoltageOutput_create(void** h) { return create(h, 4); }
API Phidget_open(void* h) { static_cast<Channel*>(h)->open = openError == 0; return openError; }
API Phidget_close(void* h) { static_cast<Channel*>(h)->open = false; return 0; }
API Phidget_delete(void** h) { delete static_cast<Channel*>(*h); *h = nullptr; --alive; return 0; }
API Phidget_setDeviceSerialNumber(void* h, int serial) { static_cast<Channel*>(h)->serial = serial; return 0; }
API Phidget_setChannel(void* h, int index) { static_cast<Channel*>(h)->index = index; return 0; }
API Phidget_setHubPort(void*, int) { return 0; }
API Phidget_setIsHubPortDevice(void*, int) { return 0; }
API Phidget_getAttached(void* h, int* value)
{
    auto& c = *static_cast<Channel*>(h);
    *value = attached && c.open && (c.serial < 0 || c.serial == 123456);
    return 0;
}
API Phidget_getDeviceSerialNumber(void*, int* serial) { *serial = 123456; return 0; }
API Phidget_getErrorDescription(int, const char** text) { *text = "Simulated Phidget failure"; return 0; }
static double& value(void* h) { auto& c = *static_cast<Channel*>(h); return values[c.type][c.index]; }
#ifndef PHIDGET_MOCK_MISSING_SYMBOL
API PhidgetDigitalInput_getState(void* h, int* state) { *state = value(h) != 0; return 0; }
#endif
API PhidgetDigitalOutput_getState(void* h, int* state) { *state = value(h) != 0; return 0; }
API PhidgetVoltageInput_getVoltage(void* h, double* v) { *v = value(h); return 0; }
API PhidgetVoltageRatioInput_getVoltageRatio(void* h, double* v) { *v = value(h); return 0; }
API PhidgetVoltageOutput_getVoltage(void* h, double* v) { *v = value(h); return 0; }
API PhidgetDigitalOutput_setState(void* h, int state) { if (writeError) return writeError; value(h) = state; return 0; }
API PhidgetDigitalOutput_setDutyCycle(void* h, double v) { if (writeError) return writeError; value(h) = v; return 0; }
API PhidgetVoltageOutput_setVoltage(void* h, double v) { if (writeError) return writeError; value(h) = v; return 0; }
