#include "Module/ModuleIncludes.h"

ButtplugCommand::ButtplugCommand(ButtplugModule* module, CommandContext context, var params, Multiplex* multiplex) :
    BaseCommand(module, context, params, multiplex),
    action(static_cast<Action>((int)params.getProperty("action", STOP_ALL))),
    buttplugModule(module)
{
    if (action >= STOP_DEVICE)
        deviceIndex = addIntParameter("Device Index", "Intiface device index shown in the module's Values", 0, 0);
    if (action >= VIBRATE)
    {
        featureIndex = addIntParameter("Feature Index", "Zero-based index in this command's feature array; -1 selects all matching features", -1, -1);
        value = addFloatParameter(action == LINEAR ? "Position" : action == ROTATE ? "Speed" : "Intensity",
            "Normalized actuator value", 0, 0, 1);
        linkParamToMappingIndex(value, 0);
    }
    if (action == ROTATE) clockwise = addBoolParameter("Clockwise", "Rotation direction", true);
    if (action == LINEAR) duration = addIntParameter("Duration", "Movement duration in milliseconds", 1000, 0);
}

void ButtplugCommand::triggerInternal(int multiplexIndex)
{
    const int device = deviceIndex == nullptr ? 0 : (int)getLinkedValue(deviceIndex, multiplexIndex);
    const int feature = featureIndex == nullptr ? -1 : (int)getLinkedValue(featureIndex, multiplexIndex);
    const double amount = value == nullptr ? 0 : (double)getLinkedValue(value, multiplexIndex);
    switch (action)
    {
        case START_SCANNING: buttplugModule->sendServerCommand("StartScanning"); break;
        case STOP_SCANNING: buttplugModule->sendServerCommand("StopScanning"); break;
        case REFRESH_DEVICES: buttplugModule->sendServerCommand("RequestDeviceList"); break;
        case STOP_ALL: buttplugModule->sendServerCommand("StopAllDevices"); break;
        case STOP_DEVICE: buttplugModule->stopDevice(device); break;
        case VIBRATE: buttplugModule->setScalar(device, feature, amount, "Vibrate"); break;
        case SCALAR: buttplugModule->setScalar(device, feature, amount); break;
        case ROTATE: buttplugModule->setRotation(device, feature, amount, (bool)getLinkedValue(clockwise, multiplexIndex)); break;
        case LINEAR: buttplugModule->setLinear(device, feature, amount, (int)getLinkedValue(duration, multiplexIndex)); break;
    }
}
