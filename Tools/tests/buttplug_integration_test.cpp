#include "MainIncludes.h"
#include "Module/ModuleIncludes.h"
#include <iostream>
#include <stdexcept>
#include <thread>
#include <windows.h>
#undef main

class ButtplugTestApplication : public OrganicApplication
{
public:
    ButtplugTestApplication() : OrganicApplication("Buttplug tests", false) {}
    void initialiseInternal(const String&) override {}
};

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

static void waitFor(const std::function<bool()>& condition, const char* message, int timeout = 4000)
{
    const double deadline = Time::getMillisecondCounterHiRes() + timeout;
    while (!condition() && Time::getMillisecondCounterHiRes() < deadline)
    {
        MSG message;
        while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        Thread::sleep(1);
    }
    check(condition(), message);
}

class MockIntiface
{
public:
    WsServer server;
    std::thread thread;
    std::atomic<int> port { 0 };
    std::atomic<bool> answerPings { true };
    std::atomic<int> version { 3 };
    std::atomic<int> pingTime { 400 };
    std::atomic<bool> rejectScan { false };
    CriticalSection lock;
    Array<var> received;
    std::shared_ptr<WsServer::Connection> connection;

    MockIntiface()
    {
        server.config.port = 0;
        server.config.address = "127.0.0.1";
        auto& endpoint = server.endpoint["^/intiface$"];
        endpoint.on_open = [this](auto c) { const ScopedLock guard(lock); connection = c; };
        endpoint.on_message = [this](auto c, auto message)
        {
            const var batch = JSON::parse(String(message->string()));
            if (!batch.isArray()) return;
            for (const auto& envelope : *batch.getArray())
            {
                { const ScopedLock guard(lock); received.add(envelope); }
                const auto& properties = envelope.getDynamicObject()->getProperties();
                const String type = properties.getName(0).toString();
                const var body = properties.getValueAt(0);
                const int id = body.getProperty("Id", 0);
                var response(new DynamicObject());
                response.getDynamicObject()->setProperty("Id", id);
                String reply = "Ok";
                if (type == "RequestServerInfo")
                {
                    reply = "ServerInfo";
                    response.getDynamicObject()->setProperty("ServerName", "Mock Intiface");
                    response.getDynamicObject()->setProperty("MessageVersion", version.load());
                    response.getDynamicObject()->setProperty("MaxPingTime", pingTime.load());
                }
                else if (type == "RequestDeviceList")
                {
                    reply = "DeviceList";
                    var devices;
                    devices.append(device(7));
                    response.getDynamicObject()->setProperty("Devices", devices);
                }
                else if (type == "StartScanning" && rejectScan)
                {
                    reply = "Error";
                    response.getDynamicObject()->setProperty("ErrorCode", 3);
                    response.getDynamicObject()->setProperty("ErrorMessage", "Scan test failure");
                }
                if (type != "Ping" || answerPings) c->send(encode(reply, response).toStdString());
            }
        };
        thread = std::thread([this] { server.start([this](unsigned short p) { port = p; }); });
        waitFor([this] { return port != 0; }, "mock server started");
    }

    ~MockIntiface() { server.stop(); thread.join(); }

    static var device(int index)
    {
        var data = JSON::parse(R"({"DeviceName":"Test device","DeviceMessages":{
            "ScalarCmd":[{"ActuatorType":"Vibrate","FeatureDescriptor":"Motor","StepCount":20},
                         {"ActuatorType":"Constrict","FeatureDescriptor":"Pressure","StepCount":10},
                         {"ActuatorType":"Vibrate","FeatureDescriptor":"Motor 2","StepCount":20}],
            "RotateCmd":[{"ActuatorType":"Rotate","StepCount":20}],
            "LinearCmd":[{"ActuatorType":"Position","StepCount":100}],"StopDeviceCmd":{}}})");
        data.getDynamicObject()->setProperty("DeviceIndex", index);
        return data;
    }

    static String encode(const String& type, const var& body)
    {
        var envelope(new DynamicObject());
        envelope.getDynamicObject()->setProperty(type, body);
        var batch;
        batch.append(envelope);
        return JSON::toString(batch, true);
    }

    void send(const String& type, var body)
    {
        const ScopedLock guard(lock);
        body.getDynamicObject()->setProperty("Id", 0);
        connection->send(encode(type, body).toStdString());
    }

    int count(const String& type)
    {
        const ScopedLock guard(lock);
        int result = 0;
        for (const auto& envelope : received) if (envelope.hasProperty(type)) ++result;
        return result;
    }

    var last(const String& type)
    {
        const ScopedLock guard(lock);
        for (int i = received.size(); --i >= 0;)
            if (received[i].hasProperty(type)) return received[i].getProperty(type, var());
        return {};
    }

    void close()
    {
        const ScopedLock guard(lock);
        connection->send_close(1000, "Test disconnect");
    }
};

static void testButtplug()
{
    MockIntiface server;
    auto module = std::make_unique<ButtplugModule>();
    const String path = "ws://127.0.0.1:" + String(server.port.load()) + "/intiface";
    module->serverPath->setValue(path);
    check(!module->setScalar(7, -1, 1), "commands are gated before handshake");
    waitFor([&] { return module->isConnected->boolValue() && module->deviceCount->intValue() == 1; }, "handshake and discovery");
    check(server.last("RequestServerInfo")["MessageVersion"] == var(3), "request protocol v3");
    check(module->serverName->stringValue() == "Mock Intiface", "server name feedback");
    waitFor([&] { return server.count("StartScanning") == 1; }, "automatic scanning");
    check(module->valuesCC.getControllableContainerByName("device7") != nullptr, "device feedback container");
    const String saved = JSON::toString(module->getJSONData());
    check(saved.contains(path) && !saved.contains("device7") && !saved.contains("Mock Intiface"), "save configuration without runtime device or server state");

    check(module->setScalar(7, -1, .6, "Vibrate"), "vibrate command accepted");
    waitFor([&] { return server.count("ScalarCmd") == 1; }, "vibrate command received");
    auto scalars = server.last("ScalarCmd")["Scalars"];
    check(scalars.size() == 2 && (int)scalars[0]["Index"] == 0 && (int)scalars[1]["Index"] == 2, "vibration preserves sparse feature indices");
    check((double)scalars[0]["Scalar"] == .6 && scalars[0]["ActuatorType"] == var("Vibrate"), "normalized scalar and actuator type");
    check(module->setScalar(7, 1, 5), "generic scalar command accepted");
    waitFor([&] { return server.count("ScalarCmd") == 2; }, "generic scalar received");
    scalars = server.last("ScalarCmd")["Scalars"];
    check(scalars.size() == 1 && (double)scalars[0]["Scalar"] == 1 && scalars[0]["ActuatorType"] == var("Constrict"), "scalar inferred from capabilities and clamped");
    check(!module->setScalar(7, 1, .5, "Vibrate") && !module->setRotation(7, 5, .5, true), "unsupported features rejected");
    check(!module->setScalar(99, -1, .5) && !module->setScalar(7, -2, .5), "invalid device and feature rejected");
    check(!module->setScalar(7, -1, std::numeric_limits<double>::quiet_NaN()), "NaN rejected");
    check(!module->setLinear(7, 0, .5, -1), "negative duration rejected");
    check(module->setRotation(7, 0, .25, false), "rotation accepted");
    check(module->setLinear(7, 0, .75, 250), "linear accepted");
    waitFor([&] { return server.count("LinearCmd") == 1; }, "movement received");
    check(!(bool)server.last("RotateCmd")["Rotations"][0]["Clockwise"], "rotation direction serialized");
    check((int)server.last("LinearCmd")["Vectors"][0]["Duration"] == 250, "linear duration in milliseconds");

    var params(new DynamicObject());
    params.getDynamicObject()->setProperty("action", ButtplugCommand::VIBRATE);
    ButtplugCommand command(module.get(), CommandContext::ACTION, params);
    static_cast<IntParameter*>(command.getControllableByName("Device Index", true))->setValue(7);
    static_cast<FloatParameter*>(command.getControllableByName("Intensity", true))->setValue(.4);
    command.trigger();
    waitFor([&] { return server.count("ScalarCmd") == 3; }, "action command reaches server");
    ButtplugCommand mapping(module.get(), CommandContext::MAPPING, params);
    static_cast<IntParameter*>(mapping.getControllableByName("Device Index", true))->setValue(7);
    var input;
    input.append(.8);
    mapping.setValue(input, 0);
    waitFor([&] { return server.count("ScalarCmd") == 4; }, "mapping command reaches server");
    check(std::abs((double)server.last("ScalarCmd")["Scalars"][0]["Scalar"] - .8) < .0001, "mapping input drives intensity");
    check(module->stopDevice(7), "stop device accepted");
    module->stopAllDevices->trigger();
    waitFor([&] { return server.count("StopAllDevices") == 1; }, "stop trigger reaches server");
    waitFor([&] { return server.count("Ping") >= 2; }, "keepalive continues after acknowledgments");

    server.send("ScanningFinished", var(new DynamicObject()));
    waitFor([&] { return !module->isScanning->boolValue(); }, "scanning finished feedback");
    server.rejectScan = true;
    module->startScanning->trigger();
    waitFor([&] { return module->lastError->stringValue() == "Scan test failure"; }, "scan error reported");
    check(!module->isScanning->boolValue() && module->isConnected->boolValue(), "scan failure clears scanning without disconnecting");
    server.rejectScan = false;
    { const ScopedLock guard(server.lock); server.connection->send("not JSON"); }
    waitFor([&] { return module->lastError->stringValue().contains("Invalid Buttplug JSON"); }, "malformed reply reported");
    check(module->isConnected->boolValue(), "malformed message does not destroy session");
    var error(new DynamicObject());
    error.getDynamicObject()->setProperty("ErrorCode", 3);
    error.getDynamicObject()->setProperty("ErrorMessage", "System test error");
    server.send("Error", error);
    waitFor([&] { return module->lastError->stringValue() == "System test error"; }, "unsolicited error reported");
    check(module->isConnected->boolValue(), "system error does not match an inactive handshake or ping");
    server.send("DeviceAdded", MockIntiface::device(9));
    waitFor([&] { return module->deviceCount->intValue() == 2; }, "device hotplug");
    module->refreshDevices->trigger();
    waitFor([&] { return module->deviceCount->intValue() == 1; }, "list reconciliation removes stale devices");
    var removed(new DynamicObject());
    removed.getDynamicObject()->setProperty("DeviceIndex", 7);
    server.send("DeviceRemoved", removed);
    waitFor([&] { return module->deviceCount->intValue() == 0; }, "device removal");
    check(!module->setScalar(7, -1, .5), "removed devices cannot be controlled");

    server.send("DeviceAdded", MockIntiface::device(7));
    waitFor([&] { return module->deviceCount->intValue() == 1; }, "device returns");
    server.close();
    waitFor([&] { return !module->isConnected->boolValue(); }, "disconnect feedback");
    check(module->deviceCount->intValue() == 0, "disconnect clears stale device indices");
    waitFor([&] { return module->isConnected->boolValue() && module->deviceCount->intValue() == 1; }, "automatic reconnection", 7000);

    server.answerPings = false;
    waitFor([&] { return !module->isConnected->boolValue(); }, "missing ping acknowledgment disconnects");
    check(module->lastError->stringValue().contains("ping"), "ping failure reported");
    server.answerPings = true;
    server.version = 2;
    module->reconnect->trigger();
    waitFor([&] { return module->lastError->stringValue().contains("protocol v3"); }, "incompatible protocol rejected");
    check(!module->isConnected->boolValue(), "incompatible handshake is not connected");

    server.version = 3;
    server.pingTime = 0;
    module->autoScan->setValue(false);
    const int scans = server.count("StartScanning");
    module->reconnect->trigger();
    waitFor([&] { return module->isConnected->boolValue() && module->deviceCount->intValue() == 1; }, "reconnect without scanning");
    check(server.count("StartScanning") == scans, "scan-on-connect can be disabled");
    const int stops = server.count("StopAllDevices");
    module->enabled->setValue(false);
    waitFor([&] { return !module->isConnected->boolValue(); }, "disable disconnects");
    waitFor([&] { return server.count("StopAllDevices") > stops; }, "disable sends stop before disconnect");
    check(!module->setScalar(7, -1, 1), "disabled module rejects commands");
    module->clearItem();
}

int main()
{
    ScopedJuceInitialiser_GUI gui;
    ButtplugTestApplication app;
    app.engine.reset(new ChataigneEngine());
    int result = 0;
    try { testButtplug(); std::cout << "Buttplug integration tests passed\n"; }
    catch (const std::exception& e) { std::cerr << "Buttplug integration failed: " << e.what() << '\n'; result = 1; }
    app.engine.reset();
    return result;
}
