#include "PhidgetDevice.h"
#include <cmath>
#include <map>

namespace phidgetRuntime
{
    struct Runtime
    {
        explicit Runtime(juce::DynamicLibrary&& loaded) : library(std::move(loaded)) {}
        ~Runtime()
        {
            // Phidget22 has background threads and does not finalize on DLL
            // unload. Keep one reference for the process and finalize only at
            // shutdown, after module channels have been closed.
#if JUCE_WINDOWS
            using Finalize = int (__stdcall*)(int);
#else
            using Finalize = int (*)(int);
#endif
            if (auto finalize = reinterpret_cast<Finalize>(library.getFunction("Phidget_finalize"))) finalize(0);
        }
        juce::DynamicLibrary library;
    };

    juce::DynamicLibrary* load(const juce::String& path)
    {
        static juce::CriticalSection cacheLock;
        const juce::ScopedLock guard(cacheLock);
        juce::DynamicLibrary candidate;
        // Construct the cache after DynamicLibrary's static leak detector so
        // cached wrappers are destroyed before that detector checks at exit.
        static std::map<void*, std::unique_ptr<Runtime>> runtimes;
        if (!candidate.open(path)) return nullptr;
        auto& runtime = runtimes[candidate.getNativeHandle()];
        if (runtime == nullptr) runtime.reset(new Runtime(std::move(candidate)));
        return &runtime->library;
    }
}

const char* PhidgetDevice::typeName(Type type)
{
    static const char* names[] = { "Digital Input", "Digital Output", "Voltage Input", "Voltage Ratio Input", "Voltage Output" };
    return names[type];
}

PhidgetDevice::~PhidgetDevice() { close(); }

template<typename F> bool PhidgetDevice::resolve(F& function, const char* name)
{
    function = reinterpret_cast<F>(library->getFunction(name));
    if (function != nullptr) return true;
    error = "Phidget22 library is missing " + juce::String(name) + ". Install a current Phidget22 runtime.";
    return false;
}

bool PhidgetDevice::loadLibrary()
{
    juce::StringArray paths;
    if (config.libraryPath.isNotEmpty()) paths.add(config.libraryPath);
    else
    {
#if JUCE_WINDOWS
        paths.add(juce::File::getSpecialLocation(juce::File::currentExecutableFile).getSiblingFile("phidget22.dll").getFullPathName());
        paths.add(juce::File::getSpecialLocation(juce::File::globalApplicationsDirectory).getChildFile("Phidgets/Phidget22/phidget22.dll").getFullPathName());
        paths.add("phidget22.dll");
#elif JUCE_MAC
        paths.add("/Library/Frameworks/Phidget22.framework/Phidget22");
        paths.add("/Library/Frameworks/Phidget22.framework/Versions/Current/Phidget22");
#else
        paths.add("libphidget22.so.0");
        paths.add("libphidget22.so");
        paths.add("/usr/local/lib/libphidget22.so");
#endif
    }
    bool loaded = false;
    for (const auto& path : paths)
        if ((library = phidgetRuntime::load(path)) != nullptr) { loaded = true; break; }
    if (!loaded)
    {
        error = "Phidget22 runtime not found. Install the official Phidget22 drivers, or set Library Path to the native library.";
        return false;
    }

    if (!resolve(api.open, "Phidget_open") || !resolve(api.close, "Phidget_close")
        || !resolve(api.destroy, "Phidget_delete") || !resolve(api.setSerial, "Phidget_setDeviceSerialNumber")
        || !resolve(api.setChannel, "Phidget_setChannel") || !resolve(api.getAttached, "Phidget_getAttached")
        || !resolve(api.getSerial, "Phidget_getDeviceSerialNumber") || !resolve(api.setHubPort, "Phidget_setHubPort")
        || !resolve(api.setHubPortDevice, "Phidget_setIsHubPortDevice")
        || !resolve(api.errorDescription, "Phidget_getErrorDescription")) return false;

    static const char* creators[] = { "PhidgetDigitalInput_create", "PhidgetDigitalOutput_create", "PhidgetVoltageInput_create",
        "PhidgetVoltageRatioInput_create", "PhidgetVoltageOutput_create" };
    static const char* getters[] = { "PhidgetDigitalInput_getState", "PhidgetDigitalOutput_getState", "PhidgetVoltageInput_getVoltage",
        "PhidgetVoltageRatioInput_getVoltageRatio", "PhidgetVoltageOutput_getVoltage" };
    for (int type = 0; type < NumTypes; ++type)
    {
        if (config.counts[type] == 0) continue;
        if (!resolve(api.create[type], creators[type])) return false;
        if (type < VoltageInput) { if (!resolve(api.getState[type], getters[type])) return false; }
        else if (!resolve(api.getVoltage[type - VoltageInput], getters[type])) return false;
    }
    if (config.counts[DigitalOutput] > 0 && !resolve(api.setState, "PhidgetDigitalOutput_setState")) return false;
    if (config.counts[VoltageOutput] > 0 && !resolve(api.setVoltage, "PhidgetVoltageOutput_setVoltage")) return false;
    // PWM is optional; many InterfaceKits (including the 1012) only support on/off.
    api.setDutyCycle = reinterpret_cast<decltype(api.setDutyCycle)>(library->getFunction("PhidgetDigitalOutput_setDutyCycle"));
    return true;
}

bool PhidgetDevice::check(int result, const juce::String& operation)
{
    if (result == 0) return true;
    const char* description = nullptr;
    if (api.errorDescription != nullptr) api.errorDescription(result, &description);
    error = operation + ": " + (description != nullptr ? juce::String(description) : "Phidget error " + juce::String(result));
    return false;
}

void PhidgetDevice::closeChannels()
{
    for (auto& channel : channels)
    {
        if (channel.handle == nullptr) continue;
        api.close(channel.handle);
        api.destroy(&channel.handle);
    }
    channels.clear();
}

void PhidgetDevice::close()
{
    const juce::ScopedLock guard(lock);
    closeChannels();
    library = nullptr;
    api = {};
    isOpen = false;
    probing = false;
    boundSerial = -1;
    error.clear();
}

bool PhidgetDevice::openChannels(bool probeOnly)
{
    for (int type = 0; type < NumTypes; ++type)
        for (int index = 0; index < config.counts[type]; ++index)
        {
            channels.push_back({ { static_cast<Type>(type), index } });
            auto& channel = channels.back();
            const auto label = juce::String(typeName(channel.sample.type)) + " " + juce::String(index);
            if (!check(api.create[type](&channel.handle), "Create " + label)) return false;
            if (boundSerial >= 0 && !check(api.setSerial(channel.handle, boundSerial), "Set serial")) return false;
            if (!check(api.setChannel(channel.handle, index), "Set channel")) return false;
            if (config.hubPort >= 0 && !check(api.setHubPort(channel.handle, config.hubPort), "Set hub port")) return false;
            if (!check(api.setHubPortDevice(channel.handle, config.hubPortDevice ? 1 : 0), "Set hub port mode")) return false;
            // Non-blocking open: the SDK waits for attachment and reconnects after unplugging.
            if (!check(api.open(channel.handle), "Open " + label)) return false;
            if (probeOnly) return true;
        }
    return true;
}

bool PhidgetDevice::open(const Config& wanted)
{
    const juce::ScopedLock guard(lock);
    close();
    config = wanted;
    for (int count : config.counts)
        if (count < 0 || count > maxChannels) { error = "Invalid Phidget channel count"; return false; }
    boundSerial = config.serial;
    probing = boundSerial < 0;
    if (!loadLibrary() || !openChannels(probing))
    {
        closeChannels();
        return false;
    }
    isOpen = true;
    return true;
}

PhidgetDevice::Snapshot PhidgetDevice::poll()
{
    const juce::ScopedLock guard(lock);
    Snapshot result;
    if (isOpen && probing && !channels.empty())
    {
        int attached = 0;
        if (api.getAttached(channels.front().handle, &attached) == 0 && attached)
        {
            int serial = -1;
            if (check(api.getSerial(channels.front().handle, &serial), "Discover serial"))
            {
                // Bind every channel to the same physical device. Wildcards must
                // never silently combine inputs/outputs from different boards.
                boundSerial = serial;
                closeChannels();
                probing = false;
                if (!openChannels(false)) { closeChannels(); isOpen = false; }
            }
        }
    }
    for (auto& channel : channels)
    {
        auto& sample = channel.sample;
        int attached = 0;
        const int attachmentResult = api.getAttached(channel.handle, &attached);
        sample.attached = attachmentResult == 0 && attached != 0 && !probing;
        sample.valid = false;
        if (sample.attached)
        {
            ++result.attached;
            int code;
            if (sample.type < VoltageInput)
            {
                int state = 0;
                code = api.getState[sample.type](channel.handle, &state);
                if (code == 0) sample.value = state;
            }
            else code = api.getVoltage[sample.type - VoltageInput](channel.handle, &sample.value);
            sample.valid = code == 0 && std::isfinite(sample.value);
            // UNKNOWNVAL is normal until the first sensor sample; detachment can
            // race any getter. Other errors need to remain visible to the user.
            if (code != 0 && code != 0x33 && code != 0x34)
                check(code, juce::String("Read ") + typeName(sample.type) + " " + juce::String(sample.index));
        }
        result.samples.push_back(sample);
    }
    result.serial = boundSerial;
    result.error = error;
    return result;
}

bool PhidgetDevice::write(Type type, int index, double value, bool dutyCycle)
{
    const juce::ScopedLock guard(lock);
    if (!std::isfinite(value)) { error = "Output value must be finite"; return false; }
    if (!isOpen || probing) { error = "Phidget is not connected"; return false; }
    for (auto& channel : channels)
    {
        if (channel.sample.type != type || channel.sample.index != index) continue;
        int attached = 0;
        if (api.getAttached(channel.handle, &attached) != 0 || !attached)
        { error = "Phidget output is not attached"; return false; }
        int code;
        if (type == DigitalOutput && !dutyCycle) code = api.setState(channel.handle, value != 0 ? 1 : 0);
        else if (type == DigitalOutput && dutyCycle && api.setDutyCycle != nullptr)
        {
            if (value < 0 || value > 1) { error = "Duty cycle must be between 0 and 1"; return false; }
            code = api.setDutyCycle(channel.handle, value);
        }
        else if (type == VoltageOutput && !dutyCycle) code = api.setVoltage(channel.handle, value);
        else { error = "Unsupported Phidget output operation"; return false; }
        if (!check(code, "Write output " + juce::String(index))) return false;
        error.clear();
        return true;
    }
    error = "Phidget output channel " + juce::String(index) + " is not configured";
    return false;
}
