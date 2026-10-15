#include "MainIncludes.h"
#include "TimeMachine/TimeMachineIncludes.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>
#include <windows.h>
#undef main

class CrashTestApplication : public OrganicApplication
{
public:
    CrashTestApplication() : OrganicApplication("Crash regression tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

static void pumpMessages(int milliseconds)
{
    const double end = Time::getMillisecondCounterHiRes() + milliseconds;
    do
    {
        MSG message;
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        { TranslateMessage(&message); DispatchMessage(&message); }
        Thread::sleep(1);
    } while (Time::getMillisecondCounterHiRes() < end);
}

struct CallbackListener : ParameterListener
{
    std::function<void()> callback;
    void parameterValueChanged(Parameter*) override { callback(); }
};

static void testParameterDeletion()
{
    for (int mode = 0; mode < 4; ++mode)
    {
        std::unique_ptr<Parameter> parameter;
        if (mode == 0) parameter = std::make_unique<FloatParameter>("Temporary", "", 0);
        else
        {
            auto enumeration = std::make_unique<EnumParameter>("Temporary", "");
            enumeration->addOption("First", 10)->addOption("Second", 20);
            parameter = std::move(enumeration);
        }
        WeakReference<Parameter> reference(parameter.get());
        CallbackListener deletingListener, followingListener;
        int followingCalls = 0;
        deletingListener.callback = [&] { parameter.reset(); };
        followingListener.callback = [&] { ++followingCalls; };
        parameter->addParameterListener(&deletingListener);
        parameter->addParameterListener(&followingListener);
        if (mode == 0) parameter->setValue(1);
        if (mode == 1) static_cast<EnumParameter*>(parameter.get())->setNext(true, false);
        if (mode == 2) static_cast<EnumParameter*>(parameter.get())->setPrev(true, false);
        if (mode == 3) static_cast<EnumParameter*>(parameter.get())->setValueWithData(20);
        check(reference == nullptr, "listener can delete the source parameter");
        check(followingCalls == 0, "notification stops after its source is deleted");
    }

    EnumParameter empty("Empty", "");
    empty.setNext(true, false);
    empty.setPrev(true, false);
    check(!empty.setValueAtIndex(-1) && !empty.setValueAtIndex(0), "invalid enum indices are rejected");
    empty.addOption("Only", 1);
    check(!empty.setValueAtIndex(-1), "negative index is rejected for populated enums");

    AutomationKey key;
    key.value->setValue(0.75);
    key.easing.reset();
    check(key.getValueAt(0) == 0.75f, "key with no easing returns its value");
    key.setEasing(Easing::BEZIER);
    check(key.getValueAt(0) == 0.75f, "zero-length key returns its value");
}

static void testAudioModuleDestruction()
{
    ChataigneSequence sequence;
    sequence.ltcMode->setValueWithData(ChataigneSequence::SEND);
    sequence.ltcModuleTarget->enabled = true;
    for (int i = 0; i < 3; ++i)
    {
        auto module = std::make_unique<AudioModule>();
        auto* first = static_cast<ChataigneAudioLayer*>(sequence.layerManager->addItem(
            new ChataigneAudioLayer(&sequence, var()), var(), false));
        auto* second = static_cast<ChataigneAudioLayer*>(sequence.layerManager->addItem(
            new ChataigneAudioLayer(&sequence, var()), var(), false));
        first->setAudioModule(module.get());
        second->setAudioModule(module.get());
        sequence.setLTCAudioModule(module.get());
        sequence.setMasterAudioLayer(first);
        check(sequence.ltcAudioModule == module.get() && sequence.masterAudioModule == module.get(),
            "LTC sender and audio clock share the sound card");
        module.reset();
        check(sequence.ltcAudioModule == nullptr && sequence.masterAudioModule == nullptr,
            "sound card destruction detaches LTC and sequence clock before device manager teardown");
        check(first->audioModule == nullptr && second->audioModule == nullptr,
            "all audio layers detach during sound card destruction");
        sequence.updateLTCSender();
        sequence.layerManager->clear();
    }
}

static void testPresetInterpolation()
{
    CVGroup group;
    FloatParameter source("Value", "", 0);
    group.addItemFromParameter(&source, false);
    auto* parameter = static_cast<Parameter*>(group.values.items[0]->controllable);
    parameter->setValue(10);
    auto* first = group.pm->addItem(nullptr, var(), false);
    first->values.syncValues();
    parameter->setValue(20);
    auto* second = group.pm->addItem(nullptr, var(), false);
    second->values.syncValues();
    parameter->setValue(0);

    for (int i = 0; i < 12; ++i)
        group.goToPreset(i % 2 == 0 ? first : second, 0.04f, &group.defaultInterpolation);
    check(group.waitForThreadToExit(2000), "rapidly replacing preset interpolation stops the previous worker cleanly");
    check(std::abs(parameter->doubleValue() - 20) < 0.001, "replacement reaches the final preset");
    auto* cubic = dynamic_cast<CubicEasing*>(group.interpolationCurveSnapshot->items[0]->easing.get());
    check(cubic != nullptr && cubic->uniformLUT.isEmpty(), "realtime Bezier does not build an unused lookup table");
    pumpMessages(20);

    std::thread request([&] { group.goToPreset(first, 0.04f, &group.defaultInterpolation); });
    request.join();
    pumpMessages(150);
    check(group.waitForThreadToExit(2000), "worker-originated preset request completes");
    check(std::abs(parameter->doubleValue() - 10) < 0.001, "worker-originated request reaches its target");
    group.goToPreset(second, 0, &group.defaultInterpolation);
    check(parameter->doubleValue() == 20 && !group.isThreadRunning(), "immediate preset cancels interpolation");
    group.goToPreset(first, 10, &group.defaultInterpolation);
    group.stopInterpolation();
    check(!group.isThreadRunning(), "long interpolation stops cooperatively");
}

static void testMQTTPublishing(int port)
{
    auto module = std::make_unique<MQTTClientModule>();
    module->port->setValue(port);
    module->clientId->setValue("chataigne-crash-regression");
    module->protocol->setValueWithData(MQTTTopic::RAW);
    auto* subscription = new MQTTTopic();
    subscription->topic->setValue("codex/crash-regression");
    module->topicsManager.addItem(subscription, var(), false);
    const double deadline = Time::getMillisecondCounterHiRes() + 5000;
    while (!module->isConnected->boolValue() && Time::getMillisecondCounterHiRes() < deadline) pumpMessages(10);
    check(module->isConnected->boolValue(), "MQTT connects to isolated local broker");
    pumpMessages(100);
    auto* received = module->valuesCC.getParameterByName("codex/crash-regression", true);
    check(received != nullptr, "raw MQTT subscription has a value");
    CallbackListener listener;
    int receivedCount = 0;
    bool callbacksOnMessageThread = true;
    listener.callback = [&]
    {
        ++receivedCount;
        callbacksOnMessageThread &= MessageManager::getInstance()->isThisTheMessageThread();
    };
    received->addParameterListener(&listener);

    std::atomic<int> finished{ 0 };
    std::vector<std::thread> publishers;
    for (int thread = 0; thread < 4; ++thread)
        publishers.emplace_back([&, thread]
        {
            for (int i = 0; i < 300; ++i)
            {
                module->publishMessage("codex/crash-regression", String(thread) + ":" + String(i));
                Thread::sleep(1);
            }
            ++finished;
        });
    pumpMessages(150);
    module->clientId->setValue("chataigne-crash-regression-reconnected");
    while (finished.load() < 4) pumpMessages(10);
    for (auto& publisher : publishers) publisher.join();
    const double reconnectDeadline = Time::getMillisecondCounterHiRes() + 5000;
    while (!module->isConnected->boolValue() && Time::getMillisecondCounterHiRes() < reconnectDeadline) pumpMessages(10);
    const bool reconnected = module->isConnected->boolValue();
    pumpMessages(100);
    module->publishMessage("codex/crash-regression", "finished");
    const double receiveDeadline = Time::getMillisecondCounterHiRes() + 5000;
    while (received->stringValue() != "finished" && Time::getMillisecondCounterHiRes() < receiveDeadline) pumpMessages(10);
    received->removeParameterListener(&listener);
    check(reconnected && received->stringValue() == "finished", "MQTT reconnects and delivers after concurrent publishing");
    check(receivedCount > 1 && callbacksOnMessageThread, "MQTT payload callbacks reach the model on the message thread");
    module.reset();
    pumpMessages(20);
    std::cout << "MQTT stress passed (4 publishers, 1200 messages, reconnect, UI delivery, teardown)\n";
}

int main(int argc, char** argv)
{
    ScopedJuceInitialiser_GUI gui;
    CrashTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        if (argc > 1) testMQTTPublishing(std::atoi(argv[1]));
        else
        {
            testParameterDeletion();
            testAudioModuleDestruction();
            testPresetInterpolation();
            std::cout << "Crash lifetime regression tests passed\n";
        }
    }
    catch (const std::exception& error)
    {
        std::cerr << "Crash lifetime regression failed: " << error.what() << '\n';
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
