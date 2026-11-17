#include "Module/ModuleIncludes.h"

PhidgetModule::PhidgetModule() : Module("Phidget")
{
    serialNumber = moduleParams.addIntParameter("Serial Number", "Device serial number. -1 selects the first matching board and binds all channels to it.", -1, -1, INT_MAX);
    hubPort = moduleParams.addIntParameter("Hub Port", "VINT hub port, or -1 for any port / a USB InterfaceKit such as the 1012", -1, -1, 15);
    hubPortDevice = moduleParams.addBoolParameter("Hub Port Device", "Enable only when using the hub port itself as a digital or analog channel", false);
    libraryPath = moduleParams.addStringParameter("Library Path", "Optional absolute path to phidget22.dll, the Phidget22 framework binary, or libphidget22.so", "");
    pollInterval = moduleParams.addIntParameter("Poll Interval", "Read current channel states every this many milliseconds", 20, 5, 1000);

    for (int type = 0; type < PhidgetDevice::NumTypes; ++type)
    {
        const String name = PhidgetDevice::typeName(static_cast<PhidgetDevice::Type>(type));
        channelCounts[type] = moduleParams.addIntParameter(name + " Count", "Number of " + name.toLowerCase() + " channels to open, starting at channel 0", type < 2 ? 16 : 0, 0, PhidgetDevice::maxChannels);
        auto* group = valueGroups.add(new ControllableContainer(name + "s"));
        group->saveAndLoadRecursiveData = false;
        valuesCC.addChildControllableContainer(group);
    }
    isConnected = moduleParams.addBoolParameter("Is Connected", "All configured channels are attached", false);
    attachedChannels = moduleParams.addIntParameter("Attached Channels", "Number of channels currently attached", 0, 0, PhidgetDevice::maxChannels * PhidgetDevice::NumTypes);
    connectedSerial = moduleParams.addIntParameter("Connected Serial", "Serial number of the selected physical board", -1, -1, INT_MAX);
    status = moduleParams.addStringParameter("Status", "Phidget connection status or the most recent SDK error", "Waiting");
    for (Controllable* c : { static_cast<Controllable*>(isConnected), static_cast<Controllable*>(attachedChannels),
                           static_cast<Controllable*>(connectedSerial), static_cast<Controllable*>(status) })
    {
        c->setControllableFeedbackOnly(true);
        c->isSavable = false;
    }
    connectionFeedbackRef = isConnected;
    reconnect = moduleParams.addTrigger("Reconnect", "Retry loading the runtime and reopen the configured channels; auto selection searches again");

    defManager->add(CommandDefinition::createDef(this, "", "Set Digital Output", &PhidgetCommand::create, CommandContext::BOTH)->addParam("action", PhidgetCommand::SetDigital));
    defManager->add(CommandDefinition::createDef(this, "", "Set Duty Cycle", &PhidgetCommand::create, CommandContext::BOTH)->addParam("action", PhidgetCommand::SetDutyCycle));
    defManager->add(CommandDefinition::createDef(this, "", "Set Voltage Output", &PhidgetCommand::create, CommandContext::BOTH)->addParam("action", PhidgetCommand::SetVoltage));
    setupIOConfiguration(true, true);
    // Configuration and value hierarchy changes run on the message thread.
    startTimer(20);
}

PhidgetModule::~PhidgetModule()
{
    shuttingDown = true;
    stopTimer();
    device.close();
    for (auto* group : valueGroups) valuesCC.removeChildControllableContainer(group);
}

void PhidgetModule::clearItem()
{
    shuttingDown = true;
    stopTimer();
    device.close();
    isConnected->setValue(false);
    attachedChannels->setValue(0);
    Module::clearItem();
}

void PhidgetModule::onContainerParameterChangedInternal(Parameter* p)
{
    Module::onContainerParameterChangedInternal(p);
    if (p == enabled)
    {
        // Close immediately when disabled so commands cannot reach the hardware.
        if (!enabled->boolValue()) device.close();
        configurationDirty = true;
    }
}

void PhidgetModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
    Module::onControllableFeedbackUpdateInternal(cc, c);
    if (c == pollInterval) startTimer(pollInterval->intValue());
    if (c == serialNumber || c == hubPort || c == hubPortDevice || c == libraryPath || c == reconnect)
        configurationDirty = true;
    for (auto* count : channelCounts) if (c == count) configurationDirty = true;
}

void PhidgetModule::afterLoadJSONDataInternal()
{
    Module::afterLoadJSONDataInternal();
    configurationDirty = true;
}

void PhidgetModule::configure()
{
    device.close();
    PhidgetDevice::Config config;
    config.libraryPath = libraryPath->stringValue().trim();
    config.serial = serialNumber->intValue();
    config.hubPort = hubPort->intValue();
    config.hubPortDevice = hubPortDevice->boolValue();
    expectedChannels = 0;
    for (int type = 0; type < PhidgetDevice::NumTypes; ++type)
    {
        config.counts[type] = channelCounts[type]->intValue();
        expectedChannels += config.counts[type];
        auto* group = valueGroups[type];
        // Preserve existing parameter identities for mappings and scripts.
        while (channelValues[type].size() > config.counts[type])
            group->removeControllable(channelValues[type].removeAndReturn(channelValues[type].size() - 1));
        while (channelValues[type].size() < config.counts[type])
        {
            int index = channelValues[type].size();
            const String name = "Channel " + String(index);
            const String description = String(PhidgetDevice::typeName(static_cast<PhidgetDevice::Type>(type))) + " " + String(index);
            Parameter* value = type < PhidgetDevice::VoltageInput
                ? static_cast<Parameter*>(group->addBoolParameter(name, description, false))
                : static_cast<Parameter*>(group->addFloatParameter(name, description + (type == PhidgetDevice::VoltageRatioInput ? " (ratio)" : " (volts)"), 0));
            value->setControllableFeedbackOnly(true);
            value->isSavable = false;
            channelValues[type].add(value);
        }
        group->hideInEditor = config.counts[type] == 0;
    }
    isConnected->setValue(false);
    attachedChannels->setValue(0);
    connectedSerial->setValue(-1);
    startTimer(pollInterval->intValue());
    if (enabled->boolValue() && expectedChannels > 0) device.open(config);
}

void PhidgetModule::timerCallback()
{
    if (shuttingDown || isCurrentlyLoadingData) return;
    if (configurationDirty.exchange(false)) configure();
    if (shuttingDown) return;
    const auto snapshot = device.poll();
    const bool active = enabled->boolValue();
    attachedChannels->setValue(snapshot.attached);
    connectedSerial->setValue(snapshot.serial);
    isConnected->setValue(active && expectedChannels > 0 && snapshot.attached == expectedChannels);
    String message;
    if (!active) message = "Disabled";
    else if (expectedChannels == 0) message = "No channels configured";
    else if (snapshot.error.isNotEmpty()) message = snapshot.error;
    else if (isConnected->boolValue()) message = "Connected";
    else message = "Waiting for channels (" + String(snapshot.attached) + "/" + String(expectedChannels) + ")";
    status->setValue(message);
    const String warning = active ? snapshot.error : String();
    if (warning != lastWarning)
    {
        lastWarning = warning;
        setWarningMessage(warning, "Phidget");
        if (warning.isNotEmpty()) NLOGWARNING(niceName, warning);
    }
    if (!active) return;
    for (const auto& sample : snapshot.samples)
    {
        if (!sample.valid || !isPositiveAndBelow(sample.index, channelValues[sample.type].size())) continue;
        auto* value = channelValues[sample.type][sample.index];
        const var newValue = sample.type < PhidgetDevice::VoltageInput ? var(sample.value != 0) : var(sample.value);
        if (value->getValue() == newValue) continue;
        value->setValue(newValue);
        inActivityTrigger->trigger();
        if (logIncomingData->boolValue()) NLOG(niceName, String(PhidgetDevice::typeName(sample.type)) + " " + String(sample.index) + ": " + newValue.toString());
    }
}

bool PhidgetModule::setOutput(PhidgetDevice::Type type, int channel, double value, bool dutyCycle)
{
    if (shuttingDown || !enabled->boolValue() || configurationDirty.load()) return false;
    if (!device.write(type, channel, value, dutyCycle)) return false;
    outActivityTrigger->trigger();
    if (logOutgoingData->boolValue()) NLOG(niceName, String(PhidgetDevice::typeName(type)) + " " + String(channel) + " = " + String(value));
    return true;
}

PhidgetCommand::PhidgetCommand(PhidgetModule* module, CommandContext context, var params, Multiplex* multiplex) :
    BaseCommand(module, context, params, multiplex), module(module),
    action(static_cast<Action>(static_cast<int>(params.getProperty("action", SetDigital))))
{
    channel = addIntParameter("Channel", "Zero-based output channel number (1012: 0 to 15)", 0, 0, PhidgetDevice::maxChannels - 1);
    if (action == SetDigital) value = addBoolParameter("State", "Digital output state", false);
    else if (action == SetDutyCycle) value = addFloatParameter("Duty Cycle", "PWM duty cycle, only on supported hardware; the 1012 uses Set Digital Output", 0, 0, 1);
    else value = addFloatParameter("Voltage", "Output voltage in volts; the SDK enforces the hardware range", 0, -10, 10);
    linkParamToMappingIndex(value, 0);
}

void PhidgetCommand::triggerInternal(int multiplexIndex)
{
    module->setOutput(action == SetVoltage ? PhidgetDevice::VoltageOutput : PhidgetDevice::DigitalOutput,
        static_cast<int>(getLinkedValue(channel, multiplexIndex)), static_cast<double>(getLinkedValue(value, multiplexIndex)), action == SetDutyCycle);
}
