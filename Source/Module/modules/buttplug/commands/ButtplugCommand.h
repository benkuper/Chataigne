#pragma once

class ButtplugCommand : public BaseCommand
{
public:
    enum Action { START_SCANNING, STOP_SCANNING, REFRESH_DEVICES, STOP_ALL, STOP_DEVICE, VIBRATE, SCALAR, ROTATE, LINEAR };
    ButtplugCommand(ButtplugModule* module, CommandContext context, var params, Multiplex* multiplex = nullptr);
    void triggerInternal(int multiplexIndex) override;
    static BaseCommand* create(ControllableContainer* module, CommandContext context, var params, Multiplex* multiplex)
    {
        return new ButtplugCommand(static_cast<ButtplugModule*>(module), context, params, multiplex);
    }

private:
    Action action;
    ButtplugModule* buttplugModule;
    IntParameter* deviceIndex = nullptr;
    IntParameter* featureIndex = nullptr;
    FloatParameter* value = nullptr;
    BoolParameter* clockwise = nullptr;
    IntParameter* duration = nullptr;
};
