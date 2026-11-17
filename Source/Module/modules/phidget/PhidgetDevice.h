#pragma once

#include <juce_core/juce_core.h>
#include <array>
#include <vector>

// A small, runtime-loaded binding to the Phidget22 C API. No SDK is needed to
// build Chataigne; the user installs the official Phidget22 runtime/USB drivers.
class PhidgetDevice
{
public:
    enum Type { DigitalInput, DigitalOutput, VoltageInput, VoltageRatioInput, VoltageOutput, NumTypes };
    static constexpr int maxChannels = 64;

    struct Config
    {
        juce::String libraryPath;
        int serial = -1;
        int hubPort = -1;
        bool hubPortDevice = false;
        std::array<int, NumTypes> counts { 16, 16, 0, 0, 0 };
    };

    struct Sample
    {
        Type type;
        int index;
        bool attached = false;
        bool valid = false;
        double value = 0;
    };

    struct Snapshot
    {
        std::vector<Sample> samples;
        juce::String error;
        int serial = -1;
        int attached = 0;
    };

    ~PhidgetDevice();
    bool open(const Config& config);
    void close();
    Snapshot poll();
    bool write(Type type, int index, double value, bool dutyCycle = false);
    static const char* typeName(Type type);

private:
#if JUCE_WINDOWS
    #define CHATAIGNE_PHIDGET_CALL __stdcall
#else
    #define CHATAIGNE_PHIDGET_CALL
#endif
    template<typename... Args> using Function = int (CHATAIGNE_PHIDGET_CALL*)(Args...);
    using Handle = void*;
    struct Channel
    {
        Sample sample;
        Handle handle = nullptr;
    };
    struct API
    {
        Function<Handle> open = nullptr, close = nullptr;
        Function<Handle*> destroy = nullptr;
        Function<Handle, int> setSerial = nullptr, setChannel = nullptr, setHubPort = nullptr, setHubPortDevice = nullptr;
        Function<Handle, int*> getAttached = nullptr, getSerial = nullptr;
        Function<int, const char**> errorDescription = nullptr;
        std::array<Function<Handle*>, NumTypes> create {};
        std::array<Function<Handle, int*>, 2> getState {};
        std::array<Function<Handle, double*>, 3> getVoltage {};
        Function<Handle, int> setState = nullptr;
        Function<Handle, double> setVoltage = nullptr, setDutyCycle = nullptr;
    } api;

    template<typename F> bool resolve(F& function, const char* name);
    bool loadLibrary();
    bool openChannels(bool probeOnly);
    bool check(int result, const juce::String& operation);
    void closeChannels();

    juce::CriticalSection lock;
    // Process-wide runtime ownership is implemented in PhidgetDevice.cpp.
    juce::DynamicLibrary* library = nullptr;
    Config config;
    std::vector<Channel> channels;
    juce::String error;
    int boundSerial = -1;
    bool probing = false;
    bool isOpen = false;
#undef CHATAIGNE_PHIDGET_CALL
};
