#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"
#include "Common/Processor/ProcessorIncludes.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#undef main

class HeartbeatUDPTestApplication : public OrganicApplication
{
public:
    HeartbeatUDPTestApplication() : OrganicApplication("Heartbeat UDP tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* label)
{
    if (!condition) throw std::runtime_error(label);
}

struct ConditionRecorder : Condition::ConditionListener
{
    Array<bool> states;
    void conditionValidationChanged(Condition* c, int index, bool) override
    {
        states.add(c->getIsValid(index));
    }
};

struct LegacyRecorder : ParameterListener
{
    int count = 0;
    void parameterValueChanged(Parameter*) override { ++count; }
};

static void testUDP(ChataigneEngine& engine)
{
    engine.isLoadingFile = true;
    auto module = std::make_unique<UDPModule>();
    engine.isLoadingFile = false;
    check(module->readBytes().isEmpty(), "reading without a receiver is safe");
    module->receiver = std::make_unique<DatagramSocket>();
    check(module->receiver->bindToPort(0, "127.0.0.1"), "bind UDP test receiver");
    DatagramSocket sender;
    check(sender.bindToPort(0, "127.0.0.1"), "bind UDP test sender");
    const int port = module->receiver->getBoundPort();
    const char payload[] = "1,2,3\n";
    constexpr int packetSize = sizeof(payload) - 1;
    const int packetCount = UDPModule::maxPacketsPerRead + 4;
    for (int i = 0; i < packetCount; ++i)
        check(sender.write("127.0.0.1", port, payload, packetSize) == packetSize, "send test packet");
    check(module->receiver->waitUntilReady(true, 1000) == 1, "packets arrive");
    auto first = module->readBytes();
    check(first.size() == UDPModule::maxPacketsPerRead * packetSize, "UDP read stops at its packet budget");
    auto second = module->readBytes();
    check(second.size() == 4 * packetSize, "remaining datagrams are preserved for the next read");
    check(module->readBytes().isEmpty(), "empty UDP read returns promptly");
    module->startThread();
    module->signalThreadShouldExit();
    check(module->readBytes().isEmpty(), "UDP read honours shutdown");
    module.reset(); // Receiver must outlive the receive thread.

    for (int i = 0; i < 20; ++i)
    {
        engine.isLoadingFile = true;
        auto active = std::make_unique<UDPModule>();
        engine.isLoadingFile = false;
        active->localPort->setValue(40000 + i);
        active->setupReceiver();
        check(active->isThreadRunning(), "receive thread starts");
        active.reset(); // Repeated teardown while waitUntilReady() is active.
    }
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    HeartbeatUDPTestApplication app;
    app.engine.reset(new ChataigneEngine());
    try
    {
        testUDP(*static_cast<ChataigneEngine*>(app.engine.get()));

        BoolParameter tick("Tick", "", false);
        BoolParameter other("Other", "", false);
        StandardCondition condition, staleCondition;
        condition.sourceControllable = &tick;
        staleCondition.sourceControllable = &tick;
        tick.addParameterListener(&condition);
        tick.addParameterListener(&staleCondition);
        condition.updateComparatorFromSource();
        staleCondition.updateComparatorFromSource();
        condition.comparator->reference->setValue(false);
        ConditionRecorder recorder, staleRecorder;
        LegacyRecorder legacy;
        condition.addConditionListener(&recorder);
        staleCondition.addConditionListener(&staleRecorder);
        tick.addParameterListener(&legacy);

        // Block message delivery while six complete heartbeat cycles occur.
        std::thread producer([&]
        {
            for (int i = 0; i < 6; ++i)
            {
                tick.setValue(true);
                tick.setValue(false);
            }
        });
        producer.join();
        check(recorder.states.isEmpty(), "worker notifications remain on the message thread");
        // A queued notification from the old source must not evaluate a new target.
        tick.removeParameterListener(&staleCondition);
        staleCondition.sourceControllable = &other;
        other.addParameterListener(&staleCondition);
        staleCondition.updateComparatorFromSource();
        staleRecorder.states.clear();
        MessageManager::callAsync([] { MessageManager::getInstance()->stopDispatchLoop(); });
        MessageManager::getInstance()->runDispatchLoop();
        check(recorder.states.size() == 12, "all queued Boolean transitions are delivered");
        for (int i = 0; i < recorder.states.size(); ++i)
            check(recorder.states[i] == (i % 2 != 0), "captured tick edges retain their order");
        check(legacy.count == 12, "existing ParameterListener callbacks remain compatible");
        check(staleRecorder.states.isEmpty(), "old queued changes do not affect a retargeted condition");
        tick.removeParameterListener(&legacy);
        condition.removeConditionListener(&recorder);
        staleCondition.removeConditionListener(&staleRecorder);
        condition.clearItem();
        staleCondition.clearItem();
        std::cout << "Heartbeat and UDP regression tests passed\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Heartbeat/UDP regression failed: " << e.what() << "\n";
        app.engine.reset();
        return 1;
    }
    app.engine.reset();
    return 0;
}
