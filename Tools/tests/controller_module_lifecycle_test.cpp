#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"
#include "Module/modules/controller/joycon/JoyConModule.h"
#include <iostream>
#include <memory>
#include <stdexcept>
#include <windows.h>
#undef main
class ControllerTestApplication : public OrganicApplication
{
public:
    ControllerTestApplication() : OrganicApplication("Controller lifecycle tests", false) {}
    void initialiseInternal(const String&) override {}
};
static void pump(int milliseconds)
{
    const auto end = Time::getMillisecondCounterHiRes() + milliseconds;
    do {
        MSG message;
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) { TranslateMessage(&message); DispatchMessage(&message); }
        Thread::sleep(1);
    } while (Time::getMillisecondCounterHiRes() < end);
}
int main()
{
    ScopedJuceInitialiser_GUI gui;
    ControllerTestApplication app;
    app.engine.reset(new ChataigneEngine());
    for (int iteration = 0; iteration < 40; ++iteration)
    {
        auto first = std::make_unique<JoyConModule>();
        auto second = std::make_unique<JoyConModule>();
        auto wii = std::make_unique<WiimoteModule>();
        auto wii2 = std::make_unique<WiimoteModule>();
        wii2->deviceID->setValue(4);
        first->reconnectControllers->trigger();
        first->reconnectControllers->trigger();
        pump(5);
        first.reset();
        second->reconnectControllers->trigger();
        wii->smoothing->setValue(.8f);
        second->enabled->setValue(false);
        pump(5);
        if (WiimoteManager::getInstance()->getState(-1).connected || WiimoteManager::getInstance()->getState(4).connected)
            throw std::runtime_error("Invalid Wii Remote selection returned connected state");
        wii.reset(); wii2.reset(); second.reset();
        // Exercise destruction while the worker is still starting/discovering.
        WiimoteManager::deleteInstance();
        pump(2);
    }
    app.engine.reset();
    std::cout << "Controller module construction, two modules, selected ID, reconnect, disable and early teardown passed\n";
}
