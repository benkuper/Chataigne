#include "Module/ModuleIncludes.h"

ButtplugModule::ButtplugModule() : Module("Buttplug")
{
    serverPath = moduleParams.addStringParameter("Server Path", "Intiface WebSocket server, with optional ws:// or wss:// prefix", "127.0.0.1:12345");
    useSecureConnection = moduleParams.addBoolParameter("Use Secure Connection", "Connect using wss://", false);
    autoScan = moduleParams.addBoolParameter("Scan On Connect", "Start device discovery after connecting to Intiface", true);
    reconnect = moduleParams.addTrigger("Reconnect", "Reconnect to the Intiface server");
    startScanning = moduleParams.addTrigger("Start Scanning", "Ask Intiface to discover devices");
    stopScanning = moduleParams.addTrigger("Stop Scanning", "Stop device discovery");
    refreshDevices = moduleParams.addTrigger("Refresh Devices", "Request the current device list");
    stopAllDevices = moduleParams.addTrigger("Stop All Devices", "Stop every device connected to Intiface");
    isConnected = moduleParams.addBoolParameter("Connected", "Buttplug handshake completed", false);
    isScanning = moduleParams.addBoolParameter("Scanning", "Intiface is discovering devices", false);
    serverName = moduleParams.addStringParameter("Server Name", "Name reported by Intiface", "");
    deviceCount = moduleParams.addIntParameter("Device Count", "Number of connected devices", 0, 0);
    lastError = moduleParams.addStringParameter("Last Error", "Most recent connection or protocol error", "");
    for (auto* p : Array<Parameter*> { isConnected, isScanning, serverName, deviceCount, lastError })
    {
        p->setControllableFeedbackOnly(true);
        p->isSavable = false;
    }
    connectionFeedbackRef = isConnected;
    // Device indices and capabilities are discovered afresh for every session.
    includeValuesInSave = false;

    const StringArray names { "Start Scanning", "Stop Scanning", "Refresh Devices", "Stop All Devices",
                              "Stop Device", "Vibrate", "Set Scalar", "Rotate", "Linear" };
    for (int i = 0; i < names.size(); ++i)
        defManager->add(CommandDefinition::createDef(this, "", names[i], &ButtplugCommand::create,
            i < 5 ? CommandContext::ACTION : CommandContext::BOTH)->addParam("action", i));

    setupIOConfiguration(true, true);
    startTimer(20);
}

ButtplugModule::~ButtplugModule()
{
    shuttingDown = true;
    stopTimer();
    stopClient();
}

void ButtplugModule::clearItem()
{
    shuttingDown = true;
    stopTimer();
    stopClient();
    Module::clearItem();
}

void ButtplugModule::enqueue(EventType type, const String& text)
{
    const ScopedLock lock(eventLock);
    if (!shuttingDown) events.add({ type, text });
}

void ButtplugModule::connectionOpened() { enqueue(OPENED); }
void ButtplugModule::connectionClosed(int, const String& reason) { enqueue(CLOSED, reason); }
void ButtplugModule::connectionError(int, const String& message) { enqueue(ERROR, message); }
void ButtplugModule::messageReceived(const String& message) { enqueue(MESSAGE, message); }

void ButtplugModule::resetSession()
{
    socketConnected = false;
    handshakeId = scanRequestId = pingId = maxPingTime = 0;
    isConnected->setValue(false);
    isScanning->setValue(false);
    serverName->setValue("");
    devices.clear();
    valuesCC.clear();
    deviceCount->setValue(0);
}

void ButtplugModule::stopClient()
{
    const ScopedLock lock(stateLock);
    if (client != nullptr)
    {
        if (socketConnected && isConnected->boolValue()) sendRequest("StopAllDevices");
        // Callbacks only enqueue events, so joining here cannot wait on stateLock.
        client->stop();
        client->removeWebSocketListener(this);
        client.reset();
    }
    { const ScopedLock eventGuard(eventLock); events.clear(); }
    resetSession();
}

void ButtplugModule::setupClient()
{
    stopClient();
    if (shuttingDown || !enabled->boolValue() || isCurrentlyLoadingData || Engine::mainEngine->isLoadingFile) return;

    String path = serverPath->stringValue().trim();
    bool secure = useSecureConnection->boolValue();
    if (path.startsWithIgnoreCase("wss://")) { path = path.substring(6); secure = true; }
    else if (path.startsWithIgnoreCase("ws://")) { path = path.substring(5); secure = false; }

    retryTime = Time::getMillisecondCounterHiRes() + 5000;
    if (path.isEmpty()) { reportError("Set an Intiface server path."); return; }
    if (secure)
    {
#if SIMPLEWEB_SECURE_SUPPORTED
        client.reset(new SecureWebSocketClient());
#else
        reportError("Secure WebSocket connections are unavailable in this build.");
        return;
#endif
    }
    else client.reset(new SimpleWebSocketClient());
    nextMessageId = 1;
    connectTime = Time::getMillisecondCounterHiRes();
    client->addWebSocketListener(this);
    client->start(path, 5);
}

void ButtplugModule::timerCallback()
{
    if (shuttingDown || isCurrentlyLoadingData || Engine::mainEngine->isLoadingFile) return;
    const ScopedLock lock(stateLock);
    if (reconnectRequested.exchange(false)) setupClient();
    if (!enabled->boolValue())
    {
        if (client != nullptr) stopClient();
        return;
    }

    Array<Event> pending;
    { const ScopedLock eventGuard(eventLock); pending.swapWith(events); }
    for (const auto& event : pending)
    {
        if (event.type == OPENED)
        {
            socketConnected = true;
            connectTime = Time::getMillisecondCounterHiRes();
            var body(new DynamicObject());
            body.getDynamicObject()->setProperty("ClientName", "Chataigne");
            body.getDynamicObject()->setProperty("MessageVersion", 3);
            handshakeId = sendRequest("RequestServerInfo", body);
        }
        else if (event.type == MESSAGE) processMessage(event.text);
        else
        {
            if (event.type == ERROR) reportError(event.text);
            stopClient();
            retryTime = Time::getMillisecondCounterHiRes() + 5000;
            break;
        }
    }

    const double now = Time::getMillisecondCounterHiRes();
    if (client == nullptr)
    {
        if (now >= retryTime) setupClient();
    }
    else if (!isConnected->boolValue() && now - connectTime >= 10000)
    {
        reportError("Intiface connection or Buttplug handshake timed out.");
        stopClient();
        retryTime = now + 5000;
    }
    else if (isConnected->boolValue() && maxPingTime > 0)
    {
        if (pingId != 0 && now - lastPingTime >= maxPingTime)
        {
            reportError("Intiface did not acknowledge the Buttplug ping.");
            stopClient();
            retryTime = now + 5000;
        }
        else if (pingId == 0 && now - lastPingTime >= jmax(1, maxPingTime / 2))
        {
            pingId = sendRequest("Ping");
            lastPingTime = now;
        }
    }
}

int ButtplugModule::sendRequest(const String& type, var body)
{
    if (client == nullptr || !socketConnected) return 0;
    const int id = nextMessageId;
    nextMessageId = nextMessageId == std::numeric_limits<int>::max() ? 1 : nextMessageId + 1;
    body.getDynamicObject()->setProperty("Id", id);
    var envelope(new DynamicObject());
    envelope.getDynamicObject()->setProperty(type, body);
    var batch;
    batch.append(envelope);
    const String message = JSON::toString(batch, true);
    client->send(message);
    outActivityTrigger->trigger();
    if (logOutgoingData->boolValue()) NLOG(niceName, message);
    return id;
}

bool ButtplugModule::sendServerCommand(const String& type)
{
    const ScopedLock lock(stateLock);
    if (shuttingDown || !enabled->boolValue() || !isConnected->boolValue()) return false;
    if (type != "StartScanning" && type != "StopScanning" && type != "RequestDeviceList" && type != "StopAllDevices") return false;
    const int id = sendRequest(type);
    if (id != 0 && (type == "StartScanning" || type == "StopScanning"))
    {
        scanRequestId = id;
        isScanning->setValue(type == "StartScanning");
    }
    return id != 0;
}

bool ButtplugModule::stopDevice(int deviceIndex)
{
    const ScopedLock lock(stateLock);
    if (shuttingDown || !enabled->boolValue() || !isConnected->boolValue() || !devices.contains(deviceIndex)) return false;
    var body(new DynamicObject());
    body.getDynamicObject()->setProperty("DeviceIndex", deviceIndex);
    return sendRequest("StopDeviceCmd", body) != 0;
}

bool ButtplugModule::setScalar(int device, int feature, double value, const String& type)
{
    return sendActuatorCommand("ScalarCmd", device, feature, value, false, 0, type);
}

bool ButtplugModule::setRotation(int device, int feature, double speed, bool clockwise)
{
    return sendActuatorCommand("RotateCmd", device, feature, speed, clockwise, 0, {});
}

bool ButtplugModule::setLinear(int device, int feature, double position, int durationMs)
{
    return sendActuatorCommand("LinearCmd", device, feature, position, false, durationMs, {});
}

bool ButtplugModule::sendActuatorCommand(const String& type, int deviceIndex, int featureIndex,
    double value, bool clockwise, int durationMs, const String& actuatorType)
{
    const ScopedLock lock(stateLock);
    if (shuttingDown || !enabled->boolValue() || !isConnected->boolValue() || !devices.contains(deviceIndex)
        || featureIndex < -1 || !std::isfinite(value) || durationMs < 0) return false;

    const var capabilities = devices[deviceIndex].getProperty("DeviceMessages", var()).getProperty(type, var());
    const auto* features = capabilities.getArray();
    if (features == nullptr) return false;
    var entries;
    for (int i = 0; i < features->size(); ++i)
    {
        if (featureIndex != -1 && featureIndex != i) continue;
        const String actualType = (*features)[i].getProperty("ActuatorType", "").toString();
        if (actuatorType.isNotEmpty() && actualType != actuatorType) continue;
        if (type == "ScalarCmd" && actualType.isEmpty()) continue;
        var entry(new DynamicObject());
        auto* object = entry.getDynamicObject();
        object->setProperty("Index", i);
        object->setProperty(type == "ScalarCmd" ? "Scalar" : type == "RotateCmd" ? "Speed" : "Position", jlimit(0.0, 1.0, value));
        if (type == "ScalarCmd") object->setProperty("ActuatorType", actualType);
        else if (type == "RotateCmd") object->setProperty("Clockwise", clockwise);
        else object->setProperty("Duration", durationMs);
        entries.append(entry);
    }
    if (!entries.isArray()) return false;
    var body(new DynamicObject());
    body.getDynamicObject()->setProperty("DeviceIndex", deviceIndex);
    body.getDynamicObject()->setProperty(type == "ScalarCmd" ? "Scalars" : type == "RotateCmd" ? "Rotations" : "Vectors", entries);
    return sendRequest(type, body) != 0;
}

void ButtplugModule::processMessage(const String& message)
{
    if (!socketConnected) return;
    inActivityTrigger->trigger();
    if (logIncomingData->boolValue()) NLOG(niceName, message);
    var data;
    const juce::Result result = JSON::parse(message, data);
    if (result.failed() || !data.isArray()) { reportError("Invalid Buttplug JSON message."); return; }
    for (const auto& envelope : *data.getArray())
    {
        const auto* object = envelope.getDynamicObject();
        if (object == nullptr || object->getProperties().size() != 1) continue;
        const auto& properties = object->getProperties();
        if (properties.getValueAt(0).isObject()) processMessage(properties.getName(0).toString(), properties.getValueAt(0));
    }
}

void ButtplugModule::processMessage(const String& type, const var& body)
{
    const int id = body.getProperty("Id", 0);
    if (type == "Error")
    {
        reportError(body.getProperty("ErrorMessage", "Unknown Buttplug error").toString());
        if (id == scanRequestId) { scanRequestId = 0; isScanning->setValue(false); }
        if ((handshakeId != 0 && id == handshakeId) || (pingId != 0 && id == pingId))
        {
            stopClient();
            retryTime = Time::getMillisecondCounterHiRes() + 5000;
        }
        return;
    }
    if (type == "ServerInfo")
    {
        if (handshakeId == 0 || id != handshakeId || isConnected->boolValue()) return;
        if ((int)body.getProperty("MessageVersion", -1) != 3 || !body.hasProperty("MaxPingTime"))
        {
            reportError("The server did not negotiate Buttplug protocol v3.");
            stopClient();
            retryTime = Time::getMillisecondCounterHiRes() + 5000;
            return;
        }
        handshakeId = 0;
        maxPingTime = jmax(0, (int)body.getProperty("MaxPingTime", 0));
        lastPingTime = Time::getMillisecondCounterHiRes();
        serverName->setValue(body.getProperty("ServerName", ""));
        isConnected->setValue(true);
        lastError->setValue("");
        clearWarning("Buttplug");
        sendServerCommand("RequestDeviceList");
        if (autoScan->boolValue()) sendServerCommand("StartScanning");
        return;
    }
    if (!isConnected->boolValue()) return;
    if (type == "Ok")
    {
        if (id == pingId) pingId = 0;
        if (id == scanRequestId) scanRequestId = 0;
    }
    else if (type == "DeviceList")
    {
        const var list = body.getProperty("Devices", var());
        if (!list.isArray()) return;
        Array<int> found;
        for (const auto& device : *list.getArray())
        {
            updateDevice(device);
            found.add((int)device.getProperty("DeviceIndex", -1));
        }
        Array<int> removed;
        for (HashMap<int, var>::Iterator it(devices); it.next();)
            if (!found.contains(it.getKey())) removed.add(it.getKey());
        for (int index : removed) removeDevice(index);
    }
    else if (type == "DeviceAdded") updateDevice(body);
    else if (type == "DeviceRemoved") removeDevice((int)body.getProperty("DeviceIndex", -1));
    else if (type == "ScanningFinished") { scanRequestId = 0; isScanning->setValue(false); }
}

void ButtplugModule::updateDevice(const var& data)
{
    const int index = data.getProperty("DeviceIndex", -1);
    if (index < 0 || !data.getProperty("DeviceMessages", var()).isObject()) return;
    devices.set(index, data);
    const String key = "device" + String(index);
    auto* container = valuesCC.getControllableContainerByName(key);
    const String name = data.getProperty("DeviceDisplayName", data.getProperty("DeviceName", "Device")).toString();
    if (container != nullptr)
    {
        container->setNiceName(name + " [" + String(index) + "]");
        static_cast<StringParameter*>(container->getControllableByName("Device Name", true))->setValue(name);
        static_cast<StringParameter*>(container->getControllableByName("Features", true))->setValue(JSON::toString(data.getProperty("DeviceMessages", var()), false));
        return;
    }
    container = new ControllableContainer(name + " [" + String(index) + "]");
    container->setCustomShortName(key);
    container->addIntParameter("Device Index", "Use this index in device commands", index, 0)->setControllableFeedbackOnly(true);
    container->addStringParameter("Device Name", "Name reported by Intiface", name)->setControllableFeedbackOnly(true);
    container->addStringParameter("Features", "Supported commands and their zero-based feature indices, in array order", JSON::toString(data.getProperty("DeviceMessages", var()), false))->setControllableFeedbackOnly(true);
    valuesCC.addChildControllableContainer(container, true);
    deviceCount->setValue(devices.size());
}

void ButtplugModule::removeDevice(int index)
{
    devices.remove(index);
    if (auto* container = valuesCC.getControllableContainerByName("device" + String(index)))
        valuesCC.removeChildControllableContainer(container);
    deviceCount->setValue(devices.size());
}

void ButtplugModule::reportError(const String& message)
{
    if (lastError->stringValue() != message) NLOGWARNING(niceName, message);
    lastError->setValue(message);
    setWarningMessage(message, "Buttplug");
}

void ButtplugModule::onContainerParameterChangedInternal(Parameter* p)
{
    Module::onContainerParameterChangedInternal(p);
    if (p == enabled) reconnectRequested = true;
}

void ButtplugModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
    Module::onControllableFeedbackUpdateInternal(cc, c);
    if (isCurrentlyLoadingData || shuttingDown) return;
    if (c == serverPath || c == useSecureConnection || c == reconnect) reconnectRequested = true;
    else if (c == startScanning) sendServerCommand("StartScanning");
    else if (c == stopScanning) sendServerCommand("StopScanning");
    else if (c == refreshDevices) sendServerCommand("RequestDeviceList");
    else if (c == stopAllDevices) sendServerCommand("StopAllDevices");
}

void ButtplugModule::afterLoadJSONDataInternal()
{
    Module::afterLoadJSONDataInternal();
    reconnectRequested = true;
}
