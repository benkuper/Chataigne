#pragma once

// Intiface owns hardware access; this module is a Buttplug JSON v3 client.
class ButtplugModule : public Module,
                       private SimpleWebSocketClientBase::Listener,
                       private Timer
{
public:
    ButtplugModule();
    ~ButtplugModule() override;

    StringParameter* serverPath;
    BoolParameter* useSecureConnection;
    BoolParameter* autoScan;
    BoolParameter* isConnected;
    BoolParameter* isScanning;
    StringParameter* serverName;
    StringParameter* lastError;
    IntParameter* deviceCount;
    Trigger* reconnect;
    Trigger* startScanning;
    Trigger* stopScanning;
    Trigger* refreshDevices;
    Trigger* stopAllDevices;

    bool sendServerCommand(const String& type);
    bool stopDevice(int deviceIndex);
    bool setScalar(int deviceIndex, int featureIndex, double value, const String& actuatorType = {});
    bool setRotation(int deviceIndex, int featureIndex, double speed, bool clockwise);
    bool setLinear(int deviceIndex, int featureIndex, double position, int durationMs);

    void clearItem() override;
    void onContainerParameterChangedInternal(Parameter* p) override;
    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
    void afterLoadJSONDataInternal() override;

    static ButtplugModule* create() { return new ButtplugModule(); }
    String getDefaultTypeString() const override { return "Buttplug"; }

private:
    enum EventType { OPENED, CLOSED, ERROR, MESSAGE };
    struct Event { EventType type; String text; };
    CriticalSection eventLock;
    Array<Event> events;
    CriticalSection stateLock;
    std::unique_ptr<SimpleWebSocketClientBase> client;
    HashMap<int, var> devices;
    std::atomic<bool> reconnectRequested { false };
    std::atomic<bool> shuttingDown { false };
    bool socketConnected = false;
    int nextMessageId = 1;
    int handshakeId = 0;
    int scanRequestId = 0;
    int pingId = 0;
    int maxPingTime = 0;
    double connectTime = 0;
    double retryTime = 0;
    double lastPingTime = 0;

    void timerCallback() override;
    void setupClient();
    void stopClient();
    void resetSession();
    void enqueue(EventType type, const String& text = {});
    void connectionOpened() override;
    void connectionClosed(int status, const String& reason) override;
    void connectionError(int status, const String& message) override;
    void messageReceived(const String& message) override;
    void processMessage(const String& message);
    void processMessage(const String& type, const var& body);
    void updateDevice(const var& data);
    void removeDevice(int index);
    void reportError(const String& message);
    int sendRequest(const String& type, var body = var(new DynamicObject()));
    bool sendActuatorCommand(const String& type, int deviceIndex, int featureIndex,
                             double value, bool clockwise, int durationMs, const String& actuatorType);
};
