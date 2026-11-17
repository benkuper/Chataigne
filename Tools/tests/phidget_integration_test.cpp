#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"
#include <iostream>
#include <stdexcept>
#include <limits>
#include <windows.h>
#undef main

class PhidgetTestApplication : public OrganicApplication
{
public:
    PhidgetTestApplication() : OrganicApplication("Phidget tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* message)
{ if (!condition) throw std::runtime_error(message); }

static void waitFor(const std::function<bool()>& condition, const char* message)
{
    const double deadline = Time::getMillisecondCounterHiRes() + 3000;
    while (!condition() && Time::getMillisecondCounterHiRes() < deadline)
    {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) { TranslateMessage(&msg); DispatchMessage(&msg); }
        Thread::sleep(1);
    }
    check(condition(), message);
}

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 4) return 2;
    ScopedJuceInitialiser_GUI gui;
    PhidgetTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        DynamicLibrary mock;
        check(mock.open(argv[1]), "load mock runtime");
        auto setAttached = reinterpret_cast<int(__stdcall*)(int)>(mock.getFunction("Mock_setAttached"));
        auto alive = reinterpret_cast<int(__stdcall*)()>(mock.getFunction("Mock_alive"));
        auto setWriteError = reinterpret_cast<int(__stdcall*)(int)>(mock.getFunction("Mock_setWriteError"));
        auto setOpenError = reinterpret_cast<int(__stdcall*)(int)>(mock.getFunction("Mock_setOpenError"));
        auto setValue = reinterpret_cast<int(__stdcall*)(int, int, double)>(mock.getFunction("Mock_setValue"));
        auto reset = reinterpret_cast<int(__stdcall*)()>(mock.getFunction("Mock_reset"));
        check(setAttached && alive && setWriteError && setOpenError && setValue && reset, "mock controls");
        reset();
        {
            PhidgetDevice device;
            PhidgetDevice::Config config;
            config.libraryPath = String(argv[1]) + ".missing";
            check(!device.open(config) && device.poll().error.isNotEmpty(), "missing library produces an actionable error");
            config.libraryPath = argv[2];
            check(!device.open(config) && device.poll().error.contains("getState"), "missing API symbol rejected");
            check(alive() == 0, "symbol failure allocates no channels");
            config.libraryPath = argv[1];
            check(device.open(config) && alive() == 1, "automatic device selection probes one channel");
            auto snapshot = device.poll();
            check(snapshot.serial == 123456 && snapshot.attached == 32 && alive() == 32, "1012 opens 16 inputs and 16 outputs on one serial");
            setValue(0, 15, 1);
            snapshot = device.poll();
            check(snapshot.samples[15].valid && snapshot.samples[15].value == 1, "last digital input is read");
            check(device.write(PhidgetDevice::DigitalOutput, 15, 1), "write last digital output");
            check(device.poll().samples[31].value == 1, "output readback");
            check(!device.write(PhidgetDevice::DigitalOutput, 16, 1), "out-of-range channel rejected");
            check(!device.write(PhidgetDevice::DigitalOutput, -1, 1), "negative channel rejected");
            check(!device.write(PhidgetDevice::DigitalOutput, 0, std::numeric_limits<double>::quiet_NaN()), "non-finite output rejected");
            setWriteError(3);
            check(!device.write(PhidgetDevice::DigitalOutput, 0, 1) && device.poll().error.contains("Simulated"), "SDK write failures are visible");
            setWriteError(0);
            check(device.write(PhidgetDevice::DigitalOutput, 0, 0) && device.poll().error.isEmpty(), "successful command clears write error");
            setAttached(0);
            snapshot = device.poll();
            check(snapshot.attached == 0 && !snapshot.samples[0].valid, "unplug invalidates samples");
            check(!device.write(PhidgetDevice::DigitalOutput, 0, 1), "detached output cannot be written");
            setAttached(1);
            check(device.poll().attached == 32, "reattachment resumes polling");
            device.close();
            check(alive() == 0, "close destroys every channel");
            setOpenError(3);
            check(!device.open(config) && alive() == 0, "partial open failure cleans up");
            setOpenError(0);
            config.serial = 999;
            check(device.open(config) && device.poll().attached == 0, "explicit serial does not attach another board");
            config.serial = 123456;
            config.counts = { 1, 1, 1, 1, 1 };
            check(device.open(config), "mixed channel configuration");
            setValue(2, 0, 3.25); setValue(3, 0, 0.625);
            snapshot = device.poll();
            check(snapshot.attached == 5 && snapshot.samples[2].value == 3.25 && snapshot.samples[3].value == 0.625, "voltage and ratio readings retain native units");
            check(device.write(PhidgetDevice::VoltageOutput, 0, 4.5) && device.poll().samples[4].value == 4.5, "voltage output");
            check(device.write(PhidgetDevice::DigitalOutput, 0, 0.25, true), "PWM on capable device");
            check(!device.write(PhidgetDevice::DigitalOutput, 0, 1.5, true), "invalid duty cycle rejected");
        }
        check(alive() == 0, "backend destruction closes all channels");
        reset();
        {
            PhidgetModule module;
            module.libraryPath->setValue(argv[1]);
            waitFor([&] { return module.isConnected->boolValue(); }, "module attaches all 1012 channels");
            check(module.attachedChannels->intValue() == 32, "module connection feedback");
            auto* inputs = module.valuesCC.getControllableContainerByName("Digital Inputs", true);
            check(inputs && inputs->controllables.size() == 16, "module exposes 16 digital input values");
            auto* input = static_cast<Parameter*>(inputs->controllables[15]);
            setValue(0, 15, 1);
            waitFor([&] { return input->boolValue(); }, "input changes propagate to Chataigne");
            var params(new DynamicObject()); params.getDynamicObject()->setProperty("action", PhidgetCommand::SetDigital);
            PhidgetCommand command(&module, CommandContext::ACTION, params, nullptr);
            static_cast<Parameter*>(command.getControllableByName("Channel", true))->setValue(15);
            static_cast<Parameter*>(command.getControllableByName("State", true))->setValue(true);
            command.triggerInternal(0);
            auto* outputs = module.valuesCC.getControllableContainerByName("Digital Outputs", true);
            waitFor([&] { return static_cast<Parameter*>(outputs->controllables[15])->boolValue(); }, "command writes selected output");
            setAttached(0);
            waitFor([&] { return !module.isConnected->boolValue(); }, "module reports unplug");
            setAttached(1);
            waitFor([&] { return module.isConnected->boolValue(); }, "module reconnects");
            module.enabled->setValue(false);
            check(alive() == 0 && !module.setOutput(PhidgetDevice::DigitalOutput, 0, 1), "disable releases hardware and blocks commands");
            module.enabled->setValue(true);
            waitFor([&] { return module.isConnected->boolValue(); }, "enable reconnects");
            module.channelCounts[0]->setValue(8);
            waitFor([&] { return inputs->controllables.size() == 8 && module.isConnected->boolValue(); }, "channel count reconfiguration");
            check(inputs->controllables[7] != nullptr, "remaining values preserved");
            const var saved = module.getJSONData();
            module.clearItem();
            check(alive() == 0, "clearItem releases hardware");
            PhidgetModule restored;
            restored.loadJSONData(saved);
            waitFor([&] { return restored.isConnected->boolValue(); }, "saved configuration reconnects");
            check(restored.channelCounts[0]->intValue() == 8 && restored.attachedChannels->intValue() == 24, "saved channel counts restored");
        }
        check(alive() == 0, "module destruction releases hardware");
        std::cout << "PASS: Phidget runtime failures, 1012 I/O, serial isolation, reconnect, commands, lifecycle and persistence\n";
        if (argc == 4)
        {
            // Exercise the real SDK ABI and worker lifetime without opening
            // channels on any actual connected board or driving outputs.
            PhidgetDevice native;
            PhidgetDevice::Config config;
            config.libraryPath = argv[3];
            config.serial = INT_MAX;
            for (int iteration = 0; iteration < 8; ++iteration)
            {
                check(native.open(config), "native Phidget22 create/open ABI");
                const auto snapshot = native.poll();
                check(snapshot.samples.size() == 32 && snapshot.error.isEmpty(), "native Phidget22 read attachment ABI");
                native.close();
            }
            std::cout << "PASS: official Phidget22 runtime, 8 cycles of 32-channel creation/open/poll/cleanup\n";
        }
    }
    catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; app.engine.reset(); return 1; }
    app.engine.reset();
    return 0;
}
