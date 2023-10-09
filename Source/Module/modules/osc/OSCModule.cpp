/*f
  ==============================================================================

	OSCModule.cpp
	Created: 29 Oct 2016 7:07:07pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"

OSCModule::OSCModule(const String& name, int defaultLocalPort, int defaultRemotePort, bool canHaveInput, bool canHaveOutput) :
	Module(name),
	Thread("OSCZeroconf"),
	localPort(nullptr),
	defaultRemotePort(defaultRemotePort),
	servus("_osc._udp"),
	receiveCC(nullptr)
{

	setupIOConfiguration(canHaveInput, canHaveOutput);
	canHandleRouteValues = canHaveOutput;


	//Receive
	if (canHaveInput)
	{
		receiveCC.reset(new EnablingControllableContainer("OSC Input"));
		receiveCC->customGetEditorFunc = &EnablingNetworkControllableContainerEditor::create;
		moduleParams.addChildControllableContainer(receiveCC.get());

		localPort = receiveCC->addIntParameter("Local Port", "Local Port to bind to receive OSC Messages", defaultLocalPort, 1, 65535);
		localPort->warningResolveInspectable = this;

		receiver.registerFormatErrorHandler(&OSCHelpers::logOSCFormatError);
		receiver.addListener(this);

		if (!Engine::mainEngine->isLoadingFile) setupReceiver();

		thruManager.reset(new ControllableContainer("Pass-through"));
		thruManager->userCanAddControllables = true;
		thruManager->customUserCreateControllableFunc = &OSCModule::createThruControllable;
	}
	else
	{
		if (receiveCC != nullptr) moduleParams.removeChildControllableContainer(receiveCC.get());
		receiveCC = nullptr;
	}

	//Send
	if (canHaveOutput)
	{
		outputManager.reset(new BaseManager<OSCOutput>("OSC Outputs"));
		outputManager->addBaseManagerListener(this);

		moduleParams.addChildControllableContainer(outputManager.get());

		outputManager->setCanBeDisabled(true);
	}
	else
	{
		if (outputManager != nullptr) removeChildControllableContainer(outputManager.get());
		outputManager = nullptr;
	}

	if (thruManager != nullptr)
	{
		moduleParams.addChildControllableContainer(thruManager.get());
	}

	//Script
	scriptObject.getDynamicObject()->setMethod("send", OSCModule::sendOSCFromScript);
	scriptObject.getDynamicObject()->setMethod("sendTo", OSCModule::sendOSCToFromScript);
	scriptObject.getDynamicObject()->setMethod("match", OSCModule::matchOSCAddrFromScript);
	scriptObject.getDynamicObject()->setMethod("register", OSCModule::registerOSCCallbackFromScript);

	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("osc");

	defManager->add(CommandDefinition::createDef(this, "", "Custom Message", &CustomOSCCommand::create));

	genericSender.connect("0.0.0.0", 1);
}

OSCModule::~OSCModule()
{
	receiver.disconnect();

	if (isThreadRunning())
	{
		stopThread(1000);
	}
}

void OSCModule::setupReceiver()
{
	receiver.disconnect();
	if (receiveCC == nullptr) return;

	if (!receiveCC->enabled->boolValue())
	{
		localPort->clearWarning();
		return;
	}

	//DBG("Local port set to : " << localPort->intValue());
	bool result = receiver.connect(localPort->intValue());

	if (result)
	{
		NLOG(niceName, "Now receiving on port : " + localPort->stringValue());
		if (!isThreadRunning() && !Engine::mainEngine->isLoadingFile) startThread();

		Array<IPAddress> ad;

		if (!isCurrentlyLoadingData)
		{
			IPAddress::findAllAddresses(ad);
			Array<String> ips;
			for (auto& a : ad) ips.add(a.toString());
			ips.sort();
			String s = "Local IPs:";
			for (auto& ip : ips) s += String("\n > ") + ip;

			NLOG(niceName, s);
		}

		localPort->clearWarning();
	}
	else
	{
		NLOGERROR(niceName, "Error binding port " + localPort->stringValue());
		localPort->setWarningMessage("Error binding port " + localPort->stringValue());
	}

}

void OSCModule::processMessage(const OSCMessage& msg)
{
	if (logIncomingData->boolValue())
	{
		String s = "";
		for (auto& a : msg) s += String(" ") + OSCHelpers::getStringArg(a);
		NLOG(niceName, msg.getAddressPattern().toString() << " :" << s);
	}

	inActivityTrigger->trigger();

	if (thruManager != nullptr)
	{
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = (TargetParameter*)c)
			{
				if (!mt->enabled) continue;
				if (OSCModule* m = (OSCModule*)(mt->targetContainer.get()))
				{
					m->sendOSC(msg);
				}
			}
		}
	}


	processMessageInternal(msg);

	if (scriptManager->items.size() > 0)
	{
		Array<var> params;
		params.add(msg.getAddressPattern().toString());
		var args = var(Array<var>()); //initialize force array
		for (auto& a : msg) {
			if (a.isBlob()) {
				var arr = var();
				auto blob = a.getBlob();
				for (int i = 0; i < blob.getSize(); i++) {
					arr.append(blob[i]);
				}
				args.append(arr);
			}
			else {
				args.append(OSCHelpers::argumentToVar(a));
			}
		}
		params.add(args);
		params.add(msg.getSenderIPAddress());
		scriptManager->callFunctionOnAllItems(oscEventId, params);

		for (auto& entry : scriptCallbacks)
			if (std::get<0>(entry).matches(msg.getAddressPattern().toString()))
				scriptManager->callFunctionOnAllItems(std::get<1>(entry), params);
	}

}

void OSCModule::setupModuleFromJSONData(var data)
{
	Module::setupModuleFromJSONData(data);

	if (receiveCC != nullptr)
	{
		if (!receiveCC->enabled->isOverriden) receiveCC->enabled->setValue(hasInput);
		receiveCC->hideInEditor = !hasInput;
	}
	if (outputManager != nullptr)
	{
		if (!outputManager->enabled->isOverriden) outputManager->enabled->setValue(hasOutput);
		outputManager->hideInEditor = !hasOutput;
	}
}

void OSCModule::itemAdded(OSCOutput* output)
{
	output->warningResolveInspectable = this;
}

void OSCModule::itemsAdded(Array<OSCOutput*> outputs)
{
	for (auto& o : outputs) o->warningResolveInspectable = this;
}

void OSCModule::setupSenders()
{
	for (auto& o : outputManager->items)
	{
		o->setupSender();
		if (o->receiver != nullptr && o->listenToOutputFeedback->boolValue())
		{
			NLOG(niceName, "Feedback enabled, listening also on port " << o->socket->getBoundPort());
			o->receiver->addListener(this);
		}
	}
}

void OSCModule::sendOSC(const OSCMessage& msg)
{
	sendOSC(msg, "");
}

void OSCModule::sendOSC(const OSCMessage& msg, String ip, int port)
{
	if (isClearing || outputManager == nullptr) return;
	if (!enabled->boolValue()) return;

	if (!outputManager->enabled->boolValue()) return;

	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Send OSC : " << msg.getAddressPattern().toString());
		for (auto& a : msg)
		{
			LOG(OSCHelpers::getStringArg(a));
		}
	}

	outActivityTrigger->trigger();

	if (ip.isNotEmpty() && port > 0)
	{
		genericSender.sendToIPAddress(ip, port, msg);
	}
	else
	{
		for (auto& o : outputManager->items) o->sendOSC(msg);
	}
}

void OSCModule::setupZeroConf()
{
	if (Engine::mainEngine->isClearing || localPort == nullptr) return;

	String nameToAdvertise;
	int portToAdvertise = 0;
	while (nameToAdvertise != niceName || portToAdvertise != localPort->intValue())
	{
		nameToAdvertise = niceName;
		portToAdvertise = localPort->intValue();

		servus.withdraw();
		sleep(400);

		if (!hasInput) return;

		//DBG("ADVERTISE");
		servus.announce(portToAdvertise, ("Chataigne - " + nameToAdvertise).toStdString());

		if (nameToAdvertise != niceName || localPort->intValue() != portToAdvertise || !hasInput)
		{
			//DBG("Name or port changed during advertise, readvertising");
		}
	}

	NLOG(niceName, "Zeroconf service created : " << nameToAdvertise << ":" << portToAdvertise);
}

var OSCModule::sendOSCFromScript(const var::NativeFunctionArgs& a)
{
	OSCModule* m = getObjectFromJS<OSCModule>(a);
	if (!m->enabled->boolValue()) return var();

	if (!checkNumArgs(m->niceName, a, 1)) return var();

	OSCHelpers::BoolMode bm = m->getBoolMode();

	try
	{
		OSCMessage msg(a.arguments[0].toString());

		for (int i = 1; i < a.numArguments; ++i)
		{
			if (a.arguments[i].isArray())
			{
				Array<var>* arr = a.arguments[i].getArray();
				for (auto& aa : *arr) msg.addArgument(OSCHelpers::varToArgument(aa, bm));
			}
			else if (a.arguments[i].isObject())
			{
				String type = a.arguments[i].getProperty("type", "").toString().toLowerCase();
				if (type.isNotEmpty())
				{
					var val = a.arguments[i].getProperty("value", 0);
					if (type == "bool") msg.addBool(val);
					else if (type == "float") msg.addFloat32(val);
					else if (type == "int") msg.addInt32(val);
					else if (type == "color") msg.addArgument(OSCHelpers::varToColorArgument(val));
					else if (type == "string") msg.addString(val.toString());
				}
			}
			else
			{
				msg.addArgument(OSCHelpers::varToArgument(a.arguments[i], bm));
			}
		}

		m->sendOSC(msg);
	}
	catch (OSCFormatError& e)
	{
		NLOGERROR(m->niceName, "Error sending message : " << e.description);
	}


	return var();
}

var OSCModule::sendOSCToFromScript(const var::NativeFunctionArgs& a)
{
	OSCModule* m = getObjectFromJS<OSCModule>(a);
	if (!m->enabled->boolValue()) return var();
	if (!checkNumArgs(m->niceName, a, 3)) return var();

	OSCHelpers::BoolMode bm = m->getBoolMode();

	try
	{
		OSCMessage msg(a.arguments[2].toString());

		for (int i = 3; i < a.numArguments; ++i)
		{
			if (a.arguments[i].isArray())
			{
				Array<var>* arr = a.arguments[i].getArray();
				for (auto& aa : *arr) msg.addArgument(OSCHelpers::varToArgument(aa, bm));
			}
			else
			{
				msg.addArgument(OSCHelpers::varToArgument(a.arguments[i], bm));
			}
		}

		m->sendOSC(msg, a.arguments[0], a.arguments[1]);
	}
	catch (OSCFormatError& e)
	{
		NLOGERROR(m->niceName, "Error sending message : " << e.description);
	}


	return var();
}

var OSCModule::matchOSCAddrFromScript(const var::NativeFunctionArgs& a)
{
	OSCModule* m = getObjectFromJS<OSCModule>(a);
	if (!checkNumArgs(m->niceName, a, 2)) return var();

	try
	{
		OSCAddress address(a.arguments[0].toString());
		OSCAddressPattern pattern(a.arguments[1].toString());

		return pattern.matches(address);
	}
	catch (OSCFormatError& e)
	{
		NLOGERROR(m->niceName, "match() function called with incorrect parameters: " << e.description);
	}


	return var();
}

var OSCModule::registerOSCCallbackFromScript(const var::NativeFunctionArgs& a)
{
	OSCModule* m = getObjectFromJS<OSCModule>(a);
	if (!checkNumArgs(m->niceName, a, 2)) return var();

	try
	{
		OSCAddressPattern pattern(a.arguments[0].toString());

		if (!Identifier::isValidIdentifier(a.arguments[1].toString()))
		{
			NLOGERROR(m->niceName, "register() function: invalid callback name " << a.arguments[1].toString());
			return var();
		}
		Identifier callbackName(a.arguments[1].toString());

		for (auto& i : m->scriptCallbacks)
			if (pattern == std::get<0>(i) && callbackName == std::get<1>(i))
				return var();

		m->scriptCallbacks.add(std::make_tuple(pattern, callbackName));
	}
	catch (OSCFormatError& e)
	{
		NLOGERROR(m->niceName, "register() function: invalid pattern: " << e.description);
	}

	return var();
}

void OSCModule::createThruControllable(ControllableContainer* cc)
{
	TargetParameter* p = new TargetParameter(cc->getUniqueNameInContainer("Output Module 1"), "Target module to send the raw data to", "");
	p->targetType = TargetParameter::CONTAINER;
	p->customGetTargetContainerFunc = &ModuleManager::showAndGetModuleOfType<OSCModule>;
	p->isRemovableByUser = true;
	p->canBeDisabledByUser = true;
	p->saveValueOnly = false;
	cc->addParameter(p);
}


void OSCModule::setupFromManualCreation()
{
	if (outputManager != nullptr && outputManager->items.isEmpty())
	{
		OSCOutput* o = outputManager->addItem(nullptr, var(), false);
		o->remotePort->setValue(defaultRemotePort);
		if (!Engine::mainEngine->isLoadingFile) setupSenders();
	}
}

void OSCModule::loadJSONDataInternal(var data)
{
	Module::loadJSONDataInternal(data);
	if (thruManager != nullptr)
	{
		for (auto& c : thruManager->controllables)
		{
			if (TargetParameter* mt = dynamic_cast<TargetParameter*>(c))
			{
				mt->targetType = TargetParameter::CONTAINER;
				mt->customGetTargetContainerFunc = &ModuleManager::showAndGetModuleOfType<OSCModule>;
				mt->isRemovableByUser = true;
				mt->canBeDisabledByUser = true;
			}
		}
	}

	setupReceiver();
	if (!isThreadRunning()) startThread();
}

void OSCModule::afterLoadJSONDataInternal()
{
	if (outputManager != nullptr)
	{
		//outputManager->loadJSONData(data.getProperty("outputs", var()));
		setupSenders();
	}
}


void OSCModule::handleRoutedModuleValue(Controllable* c, RouteParams* p)
{
	if (c == nullptr || p == nullptr) return;


	if (OSCRouteParams* op = dynamic_cast<OSCRouteParams*>(p))
	{
		try
		{
			OSCMessage m(getAddressForRoutedValue(c, op));

			if (c->type != Controllable::TRIGGER)
			{
				var v = dynamic_cast<Parameter*>(c)->getValue();

				if (c->type == Parameter::COLOR)
				{
					m.addArgument(OSCHelpers::getOSCColour(((ColorParameter*)c)->getColor()));
				}
				else
				{
					if (!v.isArray())  m.addArgument(OSCHelpers::varToArgument(v, getBoolMode()));
					else
					{
						for (int i = 0; i < v.size(); ++i) m.addArgument(OSCHelpers::varToArgument(v[i], getBoolMode()));
					}
				}

			}

			sendOSC(m);
		}
		catch (const OSCFormatError&)
		{
			NLOGERROR(niceName, "Address is invalid : " << op->address->stringValue() << "\nAddresses should always start with a forward slash");
		}
	}

}

String OSCModule::getAddressForRoutedValue(Controllable*, OSCRouteParams* op)
{
	return op->address->stringValue();
}

void OSCModule::onContainerParameterChangedInternal(Parameter* p)
{
	Module::onContainerParameterChangedInternal(p);
}

void OSCModule::onContainerNiceNameChanged()
{
	Module::onContainerNiceNameChanged();
	if (Engine::mainEngine->isLoadingFile || Engine::mainEngine->isClearing) return;
	if (!isThreadRunning()) startThread();
}

void OSCModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (outputManager != nullptr && c == outputManager->enabled)
	{
		bool rv = receiveCC != nullptr ? receiveCC->enabled->boolValue() : false;
		bool sv = outputManager->enabled->boolValue();
		for (auto& o : outputManager->items) o->setForceDisabled(!sv);
		setupIOConfiguration(rv, sv);

	}
	else if (receiveCC != nullptr && c == receiveCC->enabled)
	{
		bool rv = receiveCC->enabled->boolValue();
		bool sv = outputManager != nullptr ? outputManager->enabled->boolValue() : false;
		setupIOConfiguration(rv, sv);
		localPort->setEnabled(rv);
		if (!isCurrentlyLoadingData) setupReceiver();

	}
	else if (c == localPort)
	{
		if (!isCurrentlyLoadingData) setupReceiver();
	}
	else if (OSCOutput* o = c->getParentAs<OSCOutput>())
	{
		if (c == o->listenToOutputFeedback)
		{
			if (o->listenToOutputFeedback->boolValue())
			{
				if (o->receiver != nullptr)
				{
					NLOG(niceName, "Feedback enabled, listening also on port " << o->socket->getBoundPort());
					o->receiver->addListener(this);
				}
			}
		}
	}
}

void OSCModule::oscMessageReceived(const OSCMessage& message)
{
	if (!enabled->boolValue()) return;
	processMessage(message);
}

void OSCModule::oscBundleReceived(const OSCBundle& bundle)
{
	if (!enabled->boolValue()) return;
	for (auto& m : bundle)
	{
		processMessage(m.getMessage());
	}
}

void OSCModule::run()
{
	setupZeroConf();
}

OSCModule::OSCRouteParams::OSCRouteParams(Module* sourceModule, Controllable* c)
{
	bool sourceIsGenericOSC = sourceModule->getTypeString() == "OSC";

	String tAddress;

	if (!sourceIsGenericOSC)
	{
		tAddress = c->shortName;

		ControllableContainer* cc = c->parentContainer;
		while (cc != nullptr)
		{
			if (cc->shortName != "values")
			{
				tAddress = cc->shortName + "/" + tAddress;
			}
			Module* m = dynamic_cast<Module*>(cc);
			if (m != nullptr) break;

			cc = cc->parentContainer;
		}
	}
	else
	{
		tAddress = c->niceName; //on CustomOSCModule, niceName is the actual address
	}

	if (!tAddress.startsWithChar('/')) tAddress = "/" + tAddress;

	address = addStringParameter("Address", "Route Address", tAddress);
}



///// OSC OUTPUT

OSCOutput::OSCOutput() :
	BaseItem("OSC Output"),
	Thread("OSC output"),
	forceDisabled(false),
	senderIsConnected(false)
{
	isSelectable = false;

	useLocal = addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
	remoteHost = addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
	remoteHost->autoTrim = true;
	remoteHost->setEnabled(!useLocal->boolValue());
	remotePort = addIntParameter("Remote port", "Port on which the remote host is listening to", 9000, 1, 65535);
	listenToOutputFeedback = addBoolParameter("Listen to Feedback", "If checked, this will listen to the (randomly set) bound port of this sender. This is useful when some softwares automatically detect incoming host and port to send back messages.", false);

	if (!Engine::mainEngine->isLoadingFile) setupSender();
}

OSCOutput::~OSCOutput()
{
	stopThread(1000);
}

void OSCOutput::setForceDisabled(bool value)
{
	if (forceDisabled == value) return;
	forceDisabled = value;
	setupSender();
}

void OSCOutput::onContainerParameterChangedInternal(Parameter* p)
{
	if (p == remoteHost || p == remotePort || p == useLocal)
	{
		if (!Engine::mainEngine->isLoadingFile) setupSender();
		if (p == useLocal) remoteHost->setEnabled(!useLocal->boolValue());
	}
	else if (p == enabled)
	{
		if (!Engine::mainEngine->isLoadingFile) setupSender();
	}
	else if (p == listenToOutputFeedback)
	{
		setupSender();
	}
}

InspectableEditor* OSCOutput::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new OSCOutputEditor(this, isRoot);
}

void OSCOutput::setupSender()
{
	if (isCurrentlyLoadingData) return;

	if (isThreadRunning())
	{
		stopThread(1000);
		// Clear queue
		const ScopedLock sl(queueLock);
		while (!messageQueue.empty())
			messageQueue.pop();
	}

	senderIsConnected = false;
	sender.disconnect();
	socket.reset();

	if (receiver != nullptr) receiver->disconnect();
	receiver.reset();

	if (!enabled->boolValue() || forceDisabled || Engine::mainEngine->isClearing) return;

	String targetHost = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	socket.reset(new DatagramSocket(true));
	socket->setEnablePortReuse(true);
	socket->bindToPort(0);
	senderIsConnected = sender.connectToSocket(*socket, targetHost, remotePort->intValue());

	if (senderIsConnected)
	{
		if (listenToOutputFeedback->boolValue())
		{
			receiver.reset(new OSCReceiver());
			receiver->connectToSocket(*socket);
		}
		startThread();

		NLOG(niceName, "Now sending to " + targetHost + ":" + remotePort->stringValue());
		clearWarning();
	}
	else
	{
		NLOGWARNING(niceName, "Could not connect to " << targetHost << ":" + remotePort->stringValue());
		setWarningMessage("Could not connect to " + targetHost + ":" + remotePort->stringValue());
	}
}

void OSCOutput::sendOSC(const OSCMessage& m)
{
	if (!enabled->boolValue() || forceDisabled || !senderIsConnected) return;

	{
		const ScopedLock sl(queueLock);
		messageQueue.push(std::make_unique<OSCMessage>(m));
	}
	notify();
}


void OSCOutput::run()
{
	while (!Engine::mainEngine->isClearing && !threadShouldExit())
	{
		std::unique_ptr<OSCMessage> msgToSend;

		{
			const ScopedLock sl(queueLock);
			if (!messageQueue.empty())
			{
				msgToSend = std::move(messageQueue.front());
				messageQueue.pop();
			}
		}

		if (msgToSend)
			sender.send(*msgToSend);
		else
			wait(1000); // notify() is called when a message is added to the queue
	}

	// Clear queue
	const ScopedLock sl(queueLock);
	while (!messageQueue.empty())
		messageQueue.pop();
}
