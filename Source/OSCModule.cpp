/*
  ==============================================================================

	OSCModule.cpp
	Created: 29 Oct 2016 7:07:07pm
	Author:  bkupe

  ==============================================================================
*/

#include "OSCModule.h"
#include "EnablingNetworkControllableContainerEditor.h"

OSCModule::OSCModule(const String & name, int defaultLocalPort, int defaultRemotePort, bool canHaveInput, bool canHaveOutput) :
	Module(name)
{
	
	setupIOConfiguration(canHaveInput, canHaveOutput);
	canHandleRouteValues = canHaveOutput;

	//Receive
	if (canHaveInput)
	{
		receiveCC = new EnablingControllableContainer("OSC Input");
		receiveCC->customGetEditorFunc = &EnablingNetworkControllableContainerEditor::create;
		moduleParams.addChildControllableContainer(receiveCC);

		localPort = receiveCC->addIntParameter("Local Port", "Local Port to bind to receive OSC Messages", defaultLocalPort, 1024, 65535);
		localPort->hideInOutliner = true;
		localPort->isTargettable = false;

		receiver.addListener(this);
		setupReceiver();
	}

	//Send
	if (canHaveOutput)
	{
		sendCC = new EnablingControllableContainer("OSC Output");
		moduleParams.addChildControllableContainer(sendCC);
		
		useLocal = sendCC->addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
		remoteHost = sendCC->addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
		remoteHost->setEnabled(!useLocal->boolValue());
		remotePort = sendCC->addIntParameter("Remote port", "Port on which the remote host is listening to", defaultRemotePort, 1024, 65535);

		setupSender();
	}

	//Script
	scriptObject.setMethod(sendOSCId, OSCModule::sendOSCFromScript);
}

void OSCModule::setupReceiver()
{
	if (receiveCC == nullptr) return;

	receiver.disconnect();

	if (!receiveCC->enabled->boolValue()) return;
	bool result = receiver.connect(localPort->intValue());

	if (result)
	{
		NLOG(niceName, "Now receiving on port : " + localPort->stringValue());
	} else
	{
		NLOGERROR(niceName, "Error binding port " + localPort->stringValue());
	}

	
	Array<IPAddress> ad;
	IPAddress::findAllAddresses(ad);

	Array<String> ips;
	for (auto &a : ad) ips.add(a.toString());
	ips.sort();
	String s = "Local IPs:";
	for (auto &ip : ips) s += String("\n > ") + ip;

	NLOG(niceName, s);
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
	if (sendCC == nullptr) return;

	sender.disconnect();
	if (!sendCC->enabled->boolValue()) return;

	String targetHost = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	sender.connect(targetHost, remotePort->intValue());
	NLOG(niceName, "Now sending to " + remoteHost->stringValue()+":"+remotePort->stringValue());
}

void OSCModule::sendOSC(const OSCMessage & msg)
{
	if (!enabled->boolValue()) return;
	if (sendCC == nullptr) return;

	if (!sendCC->enabled->boolValue()) return;

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

var OSCModule::getJSONData()
{
	var data = Module::getJSONData();
	if (receiveCC != nullptr)
	{
		var inputData = receiveCC->getJSONData();
		if (!inputData.isVoid()) data.getDynamicObject()->setProperty("input", inputData);
	}

	if (sendCC != nullptr)
	{
		var outputData = sendCC->getJSONData();
		if (!outputData.isVoid()) data.getDynamicObject()->setProperty("output", outputData);
	}

	return data;
}

void OSCModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	if (receiveCC != nullptr) receiveCC->loadJSONData(data.getProperty("input", var()));
	if (sendCC != nullptr) sendCC->loadJSONData(data.getProperty("output", var()));
}


void OSCModule::handleRoutedModuleValue(Controllable * c, RouteParams * p)
{
	OSCRouteParams * op = dynamic_cast<OSCRouteParams *>(p);
	OSCMessage m(op->address->stringValue());
	if (c->type != Controllable::TRIGGER)
	{
		var v = dynamic_cast<Parameter *>(c)->getValue();
		if (!v.isArray()) m.addArgument(varToArgument(v));
		else
		{
			for (int i = 0; i < v.size(); i++) m.addArgument(varToArgument(v[i]));
		}
	}
	sendOSC(m);
}

void OSCModule::onContainerParameterChangedInternal(Parameter * p)
{
	Module::onContainerParameterChangedInternal(p);

	
}

void OSCModule::controllableFeedbackUpdate(ControllableContainer * cc, Controllable * c)
{
	Module::controllableFeedbackUpdate(cc, c);

	if (receiveCC != nullptr && c == receiveCC->enabled)
	{
		bool rv = receiveCC->enabled->boolValue();
		bool sv = sendCC->enabled->boolValue();
		setupIOConfiguration(rv,sv);
		localPort->setEnabled(rv);
		setupReceiver();

	} else if (sendCC != nullptr && c == sendCC->enabled)
	{
		bool rv = receiveCC->enabled->boolValue();
		bool sv = sendCC->enabled->boolValue();
		setupIOConfiguration(rv, sv);
		remoteHost->setEnabled(sv);
		remotePort->setEnabled(sv);
		useLocal->setEnabled(sv);
		setupSender();

	} else if (c == localPort) setupReceiver();
	else if (c == remoteHost || c == remotePort || c == useLocal)
	{
		setupSender();
		if (c == useLocal) remoteHost->setEnabled(!useLocal->boolValue());
	}
}



void OSCModule::oscMessageReceived(const OSCMessage & message)
{
	if (!enabled->boolValue()) return;
	processMessage(message);
}

void OSCModule::oscBundleReceived(const OSCBundle & bundle)
{
	if (!enabled->boolValue()) return;
	for (auto &m : bundle)
	{
		processMessage(m.getMessage());
	}
}

OSCModule::OSCRouteParams::OSCRouteParams(Module * sourceModule, Controllable * c) 
{
	bool sourceIsGenericOSC = sourceModule->getTypeString() == "OSC";

	String tAddress;

	if (!sourceIsGenericOSC)
	{
		tAddress = c->shortName;

		ControllableContainer * cc = c->parentContainer;
		while (cc != nullptr)
		{
			if (cc->shortName != "values")
			{
				tAddress = cc->shortName + "/" + tAddress;
			}
			Module * m = dynamic_cast<Module *>(cc);
			if (m != nullptr) break;

			cc = cc->parentContainer;
		}
	} else
	{
		tAddress = c->niceName; //on CustomOSCModule, niceName is the actual address
	}

	if (!tAddress.startsWithChar('/')) tAddress = "/" + tAddress;
	
	address = addStringParameter("Address", "Route Address", tAddress);
}
