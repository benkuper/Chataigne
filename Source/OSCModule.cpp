/*
  ==============================================================================

	OSCModule.cpp
	Created: 29 Oct 2016 7:07:07pm
	Author:  bkupe

  ==============================================================================
*/

#include "OSCModule.h"
#include "OSCModuleBaseEditor.h"
#include "ScriptManager.h"

OSCModule::OSCModule(const String & name, int defaultLocalPort, int defaultRemotePort) :
	Module(name)
{
	//Receive

	localPort = addIntParameter("Local Port", "Local Port to bind to receive OSC Messages", defaultLocalPort, 1024, 65535);
	localPort->hideInOutliner = true;
	localPort->isTargettable = false;

	isConnected = addBoolParameter("Is Connected", "Is the receiver bound the the local port", false);
	isConnected->isEditable = false;
	isConnected->hideInOutliner = true;
	isConnected->isTargettable = false;
	isConnected->isSavable = false;

	receiver.addListener(this);
	setupReceiver();


	//Send
	useLocal = addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
	remoteHost = addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
	remotePort = addIntParameter("Remote port", "Port on which the remote host is listening to", defaultRemotePort, 1024, 65535);

	setupSender();

	//Script
	scriptObject.setMethod(sendOSCId, OSCModule::sendOSCFromScript);
}

void OSCModule::setupReceiver()
{
	bool result = receiver.connect(localPort->intValue());
	isConnected->setValue(result);

	Array<IPAddress> ad;
	IPAddress::findAllAddresses(ad);
	String s = "Local IPs:";
	for (auto &a : ad) s += String("\n > ") + a.toString();
}

float OSCModule::getFloatArg(OSCArgument a)
{
	if (a.isFloat32()) return a.getFloat32();
	if (a.isInt32()) return (float)a.getInt32();
	if (a.isString()) return a.getString().getFloatValue();
	return 0;
}

int OSCModule::getIntArg(OSCArgument a)
{
	if (a.isInt32()) return a.getInt32();
	if (a.isFloat32()) return roundFloatToInt(a.getFloat32());
	if (a.isString()) return a.getString().getIntValue();
	return 0;
}

String OSCModule::getStringArg(OSCArgument a)
{
	if (a.isString()) return a.getString();
	if (a.isInt32()) return String(a.getInt32());
	if (a.isFloat32()) return String(a.getFloat32());
	return String::empty;
}

void OSCModule::processMessage(const OSCMessage & msg)
{
	if (logIncomingData->boolValue())
	{
		String s = "";
		for (auto &a : msg) s += String(" ") + getStringArg(a);
		NLOG(niceName, msg.getAddressPattern().toString() << " :" << s);
	}

	inActivityTrigger->trigger();
	processMessageInternal(msg);

	if (scriptManager->items.size() > 0)
	{
		Array<var> params;
		params.add(msg.getAddressPattern().toString());
		var args;
		for (auto &a : msg) args.append(OSCModule::argumentToVar(a));
		params.add(args);
		scriptManager->callFunctionOnAllItems(oscEventId, params);
	}

}


void OSCModule::setupSender()
{
	sender.connect(remoteHost->stringValue(), remotePort->intValue());
}

void OSCModule::sendOSC(const OSCMessage & msg)
{
	if (!enabled->boolValue()) return;

	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Send OSC : " << msg.getAddressPattern().toString());
		for (auto &a : msg)
		{
			LOG(getStringArg(a));
		}
	}

	outActivityTrigger->trigger();
	sender.send(msg);
}

var OSCModule::sendOSCFromScript(const var::NativeFunctionArgs & a)
{
	OSCModule * m = getObjectFromJS<OSCModule>(a);
	if (!m->enabled->boolValue()) return var();

	if (a.numArguments == 0) return var();

	OSCMessage msg(a.arguments[0].toString());

	for (int i = 1; i < a.numArguments; i++)
	{
		if (a.arguments[i].isArray())
		{
			Array<var> * arr = a.arguments[i].getArray();
			for (auto &aa : *arr) msg.addArgument(varToArgument(aa));
		}else
		{
			msg.addArgument(varToArgument(a.arguments[i]));
		}
	}

	m->sendOSC(msg);

	return var();
}


OSCArgument OSCModule::varToArgument(const var & v)
{
	if (v.isBool()) return OSCArgument(((bool)v) ? 1 : 0);
	else if (v.isInt()) return OSCArgument((int)v);
	else if (v.isInt64()) return OSCArgument((int)v);
	else if (v.isDouble()) return OSCArgument((float)v);
	else if (v.isString()) return OSCArgument(v.toString());

	jassert(false);
	return OSCArgument("error");
}

var OSCModule::argumentToVar(const OSCArgument & a)
{
	if (a.isFloat32()) return var(a.getFloat32());
	else if (a.isInt32()) return var(a.getInt32());
	else if (a.isString()) return var(a.getString());
	return var("error");
}

void OSCModule::onContainerParameterChangedInternal(Parameter * p)
{
	Module::onContainerParameterChangedInternal(p);
	if (p == localPort) setupReceiver();
	else if (p == remoteHost || p == remotePort) setupSender();
}


void OSCModule::oscMessageReceived(const OSCMessage & message)
{
	if (!enabled->boolValue()) return;
	processMessage(message);
}

InspectableEditor * OSCModule::getEditor(bool isRoot)
{
	return new OSCModuleBaseEditor(this, isRoot);
}
