#pragma once

#include "PhidgetDevice.h"

class PhidgetModule : public Module, private Timer
{
public:
    PhidgetModule();
    ~PhidgetModule() override;

    StringParameter* libraryPath;
    IntParameter* serialNumber;
    IntParameter* hubPort;
    BoolParameter* hubPortDevice;
    IntParameter* pollInterval;
    std::array<IntParameter*, PhidgetDevice::NumTypes> channelCounts {};
    BoolParameter* isConnected;
    IntParameter* attachedChannels;
    IntParameter* connectedSerial;
    StringParameter* status;
    Trigger* reconnect;

    bool setOutput(PhidgetDevice::Type type, int channel, double value, bool dutyCycle = false);
    void clearItem() override;
    void onContainerParameterChangedInternal(Parameter* p) override;
    void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;
    void afterLoadJSONDataInternal() override;

    String getDefaultTypeString() const override { return "Phidget"; }
    static PhidgetModule* create() { return new PhidgetModule(); }

private:
    PhidgetDevice device;
    OwnedArray<ControllableContainer> valueGroups;
    std::array<Array<Parameter*>, PhidgetDevice::NumTypes> channelValues;
    std::atomic<bool> configurationDirty { true };
    std::atomic<bool> shuttingDown { false };
    String lastWarning;
    int expectedChannels = 0;
    void timerCallback() override;
    void configure();
};

class PhidgetCommand : public BaseCommand
{
public:
    enum Action { SetDigital, SetDutyCycle, SetVoltage };
    PhidgetCommand(PhidgetModule* module, CommandContext context, var params, Multiplex* multiplex);
    void triggerInternal(int multiplexIndex) override;
    static BaseCommand* create(ControllableContainer* cc, CommandContext context, var params, Multiplex* multiplex = nullptr)
    { return new PhidgetCommand(static_cast<PhidgetModule*>(cc), context, params, multiplex); }

private:
    PhidgetModule* module;
    Action action;
    IntParameter* channel;
    Parameter* value;
};
