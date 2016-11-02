/*
  ==============================================================================

    OSCInput.cpp
    Created: 29 Oct 2016 7:07:07pm
    Author:  bkupe

  ==============================================================================
*/

#include "OSCInput.h"

OSCInput::OSCInput(const String & name) :
	Input(name)
{

	localPort = addIntParameter("Local Port", "Local Port to bind to receive OSC Messages", 12000, 1024, 65535);
	localPort->hideInOutliner = true;
	localPort->isTargettable = false;

	isConnected = addBoolParameter("Is Receiving", "Is the receiver bound the the local port", false);
	isConnected->isEditable = false;
	isConnected->hideInOutliner = true;
	isConnected->isTargettable = false;

	receiver.addListener(this);
	setupReceiver();
}

void OSCInput::setupReceiver()
{
	if(isConnected->boolValue()) receiver.disconnect();
	bool result = receiver.connect(localPort->intValue());;
	isConnected->setValue(result);

	Array<IPAddress> ad;
	IPAddress::findAllAddresses(ad);
	String s = "Local IPs:";
	for (auto &a : ad) s += String("\n > ") + a.toString();
	NLOG(niceName, s);
}

float OSCInput::getFloatArg(OSCArgument a)
{
	if (a.isFloat32()) return a.getFloat32();
	if (a.isInt32()) return (float)a.getInt32();
	if (a.isString()) return a.getString().getFloatValue();
	return 0;
}

int OSCInput::getIntArg(OSCArgument a)
{
	if (a.isInt32()) return a.getInt32();
	if (a.isFloat32()) return roundFloatToInt(a.getFloat32());
	if (a.isString()) return a.getString().getIntValue();
	return 0;
}

String OSCInput::getStringArg(OSCArgument a)
{
	if (a.isString()) return a.getString();
	if (a.isInt32()) return String(a.getInt32());
	if (a.isFloat32()) return String(a.getFloat32());
	return String::empty;
}

void OSCInput::processMessage(const OSCMessage & msg)
{
	if (logIncomingData->boolValue())
	{
		String s = "";
		for (auto &a : msg) s += String(" ") + getStringArg(a);
		NLOG(niceName, msg.getAddressPattern().toString() << " :" << s);
	}

	activityTrigger->trigger();

	processMessageInternal(msg);
}


void OSCInput::onContainerParameterChangedInternal(Parameter * p)
{
	Input::onContainerParameterChangedInternal(p);
	if (p == localPort) setupReceiver();
}

void OSCInput::oscMessageReceived(const OSCMessage & message)
{
	if (!enabled->boolValue()) return;
	processMessage(message);
}
