/*
  ==============================================================================

	GenericOSCQueryModule.cpp
	Created: 28 Feb 2019 10:33:17pm
	Author:  bkupe

  ==============================================================================
*/

#include "Module/ModuleIncludes.h"
#include "GenericOSCQueryModule.h"

GenericOSCQueryModule::GenericOSCQueryModule(const String& name, int defaultRemotePort) :
	Module(name),
	Thread("OSCQuery"),
	useLocal(nullptr),
	remoteHost(nullptr),
	remotePort(nullptr),
	isUpdatingStructure(false),
	hasListenExtension(false)
{
	alwaysShowValues = true;
	canHandleRouteValues = true;

	includeValuesInSave = true;

	setupIOConfiguration(true, true);

	keepValuesOnSync = moduleParams.addBoolParameter("Keep Values On Sync", "If checked, this will force keeping the current values when syncing the OSCQuery remote data structure.", false);
	syncTrigger = moduleParams.addTrigger("Sync Data", "Sync the data");
	serverName = moduleParams.addStringParameter("Server Name", "The name of the OSCQuery server, if provided", "");
	onlySyncSameName = moduleParams.addBoolParameter("Only sync from same name", "If checked, this will not sync if the server name is different", true);
	useAddressForNaming = moduleParams.addBoolParameter("Use address for naming", "If checked, the parameter's ADDRESS field will be used for naming instead of the DESCRIPTION field", false);

	isConnected = moduleParams.addBoolParameter("Is Connected", "Is Connected to server's websocket", false);
	isConnected->hideInEditor = true;
	connectionFeedbackRef = isConnected;

	listenAllTrigger = moduleParams.addTrigger("Listen to all", "This will automatically enable listen to all containers");
	listenNoneTrigger = moduleParams.addTrigger("Listen to none", "This will automatically disable listen to all containers");

	sendCC.reset(new OSCQueryOutput(this));
	moduleParams.addChildControllableContainer(sendCC.get());

	useLocal = sendCC->addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
	remoteHost = sendCC->addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
	remoteHost->autoTrim = true;
	remoteHost->setEnabled(!useLocal->boolValue());
	remotePort = sendCC->addIntParameter("Remote port", "Port on which the remote host is listening to", defaultRemotePort, 1, 65535);
	remoteOSCPort = sendCC->addIntParameter("Custom OSC Port", "If enabled, this will override the port to send OSC to, default is sending to the OSCQuery port", defaultRemotePort, 1, 65535);
	remoteOSCPort->canBeDisabledByUser = true;
	remoteOSCPort->setEnabled(false);

	remoteWSPort = sendCC->addIntParameter("Custom Websocket Port", "If enabled, this will override the port to send Websocket to, default is sending to the OSCQuery port", defaultRemotePort, 1, 65535);
	remoteWSPort->canBeDisabledByUser = true;
	remoteWSPort->setEnabled(false);

	//Script
	scriptObject.getDynamicObject()->setMethod("send", GenericOSCQueryModule::sendOSCFromScript);

	defManager->add(CommandDefinition::createDef(this, "", "Set Value", &GenericControllableCommand::create, CommandContext::BOTH)->addParam("action", GenericControllableCommand::SET_VALUE)->addParam("root", (int64)&valuesCC));
	defManager->add(CommandDefinition::createDef(this, "", "Go to Value", &GenericControllableCommand::create, CommandContext::BOTH)->addParam("action", GenericControllableCommand::GO_TO_VALUE)->addParam("root", (int64)&valuesCC));
	defManager->add(CommandDefinition::createDef(this, "", "Trigger", &GenericControllableCommand::create, CommandContext::BOTH)->addParam("action", GenericControllableCommand::TRIGGER)->addParam("root", (int64)&valuesCC));

	defManager->add(CommandDefinition::createDef(this, "", "Custom Message", &CustomOSCCommand::create, CommandContext::BOTH));

	sender.connect("0.0.0.0", 0);

	if (Engine::mainEngine->isLoadingFile) Engine::mainEngine->addEngineListener(this);

	startTimer(5000);
}

GenericOSCQueryModule::~GenericOSCQueryModule()
{
	if (Engine::mainEngine != nullptr) Engine::mainEngine->removeEngineListener(this);

	if (wsClient != nullptr) wsClient->stop();
	stopThread(2000);
	valuesCC.clear();
}

void GenericOSCQueryModule::setupWSClient()
{
	isConnected->setValue(false);
	if (wsClient != nullptr) wsClient->stop();
	wsClient.reset();
	if (isCurrentlyLoadingData || !hasListenExtension) return;

	if (!enabled->boolValue()) return;
	wsClient.reset(new SimpleWebSocketClient());
	wsClient->addWebSocketListener(this);

	String host = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	String wsPort = remoteWSPort->enabled ? remoteWSPort->stringValue() : remotePort->stringValue();

	String url = host + ":" + wsPort + "/";

	if (wsPort != remoteHost->stringValue()) LOG("Connecting to custom WS Port : " + url + "...");
	wsClient->start(url);
}

void GenericOSCQueryModule::sendOSC(const OSCMessage& m)
{
	if (!enabled->boolValue()) return;

	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Send OSC : " << m.getAddressPattern().toString());
		for (auto& a : m) LOG(OSCHelpers::getStringArg(a));
	}

	outActivityTrigger->trigger();

	String host = useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue();
	int port = remoteOSCPort->enabled ? remoteOSCPort->intValue() : remotePort->intValue();
	sender.sendToIPAddress(host, port, m);
}

void GenericOSCQueryModule::sendOSCForControllable(Controllable* c)
{
	if (!enabled->boolValue()) return;
	if (isUpdatingStructure) return;
	if (isCurrentlyLoadingData) return;
	if (noFeedbackList.contains(c)) return;


	String s = c->getControlAddress(&valuesCC);
	try
	{
		OSCMessage m(s);
		if (c->type != Controllable::TRIGGER)
		{
			Parameter* p = (Parameter*)c;
			if (p->value.isArray() && p->type != Controllable::COLOR)
			{
				for (int i = 0; i < p->value.size(); ++i)
				{
					m.addArgument(OSCHelpers::varToArgument(p->value[i], getBoolMode()));
				}
			}
			else
			{
				m.addArgument(OSCHelpers::varToArgument(p->value, getBoolMode()));
			}
		}
		sendOSC(m);
	}
	catch (OSCFormatError& e)
	{
		NLOGERROR(niceName, "Can't send to address " << s << " : " << e.description);
	}
}

var GenericOSCQueryModule::sendOSCFromScript(const var::NativeFunctionArgs& a)
{
	GenericOSCQueryModule* m = getObjectFromJS<GenericOSCQueryModule>(a);
	if (!m->enabled->boolValue()) return var();

	if (a.numArguments == 0) return var();

	try
	{
		OSCMessage msg(a.arguments[0].toString());

		for (int i = 1; i < a.numArguments; ++i)
		{
			if (a.arguments[i].isArray())
			{
				Array<var>* arr = a.arguments[i].getArray();
				for (auto& aa : *arr) msg.addArgument(varToArgument(aa));
			}
			else
			{
				msg.addArgument(varToArgument(a.arguments[i]));
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


OSCArgument GenericOSCQueryModule::varToArgument(const var& v)
{
	if (v.isBool()) return OSCArgument(((bool)v) ? 1 : 0);
	else if (v.isInt()) return OSCArgument((int)v);
	else if (v.isInt64()) return OSCArgument((int)v);
	else if (v.isDouble()) return OSCArgument((float)v);
	else if (v.isString()) return OSCArgument(v.toString());
	jassert(false);
	return OSCArgument("error");
}

void GenericOSCQueryModule::syncData()
{
	if (isCurrentlyLoadingData || Engine::mainEngine->isLoadingFile) return;

	startThread();
}

void GenericOSCQueryModule::updateTreeFromData(var data)
{
	if (data.isVoid()) return;

	isUpdatingStructure = true;

	Array<String> enableListenContainers;
	Array<String> expandedContainers;
	Array<WeakReference<ControllableContainer>> containers = valuesCC.getAllContainers(true);

	if (!keepValuesOnSync->boolValue())
	{
		for (auto& cc : containers)
		{
			if (OSCQueryHelpers::OSCQueryValueContainer* gcc = dynamic_cast<OSCQueryHelpers::OSCQueryValueContainer*>(cc.get()))
			{
				if (gcc->enableListen->boolValue())
				{
					enableListenContainers.add(gcc->getControlAddress(&valuesCC));
					gcc->enableListen->setValue(false);
					if (!gcc->editorIsCollapsed) expandedContainers.add(gcc->getControlAddress(&valuesCC));
				}
			}
		}
	}


	var vData(new DynamicObject());
	if (keepValuesOnSync->boolValue())
	{
		Array<WeakReference<Parameter>> params = valuesCC.getAllParameters(true);
		for (auto& p : params)
		{
			if (p->isOverriden) vData.getDynamicObject()->setProperty(p->getControlAddress(&valuesCC), p->value);
		}

		for (auto& cc : containers)
		{
			if (OSCQueryHelpers::OSCQueryValueContainer* gcc = dynamic_cast<OSCQueryHelpers::OSCQueryValueContainer*>(cc.get()))
			{
				if (gcc->enableListen->boolValue()) gcc->enableListen->setValue(true, false, true); //force relistening
			}
		}
	}

	//valuesCC.clear();

	OSCQueryHelpers::updateContainerFromData(&valuesCC, data, useAddressForNaming->boolValue());

	isUpdatingStructure = false;

	if (keepValuesOnSync->boolValue())
	{
		NamedValueSet nvs = vData.getDynamicObject()->getProperties();
		for (auto& nv : nvs)
		{
			if (Parameter* p = dynamic_cast<Parameter*>(valuesCC.getControllableForAddress(nv.name.toString())))
			{
				p->setValue(nv.value);
			}
		}
	}
	else
	{
		for (auto& addr : enableListenContainers)
		{
			if (OSCQueryHelpers::OSCQueryValueContainer* gcc = dynamic_cast<OSCQueryHelpers::OSCQueryValueContainer*>(valuesCC.getControllableContainerForAddress(addr)))
			{
				gcc->enableListen->setValue(true, false, true);
			}
		}

		for (auto& addr : expandedContainers)
		{
			if (ControllableContainer* cc = valuesCC.getControllableContainerForAddress(addr))
			{
				cc->editorIsCollapsed = false;
				cc->queuedNotifier.addMessage(new ContainerAsyncEvent(ContainerAsyncEvent::ControllableContainerCollapsedChanged, cc)); //should move to a setCollapsed from ControllableContainer.cpp
			}
		}
	}

	treeData = data;
}


void GenericOSCQueryModule::updateAllListens()
{
	Array<WeakReference<ControllableContainer>> containers = valuesCC.getAllContainers(true);
	for (auto& cc : containers)
	{
		if (OSCQueryHelpers::OSCQueryValueContainer* gcc = dynamic_cast<OSCQueryHelpers::OSCQueryValueContainer*>(cc.get()))
		{
			updateListenToContainer(gcc, true);
		}
	}
}

void GenericOSCQueryModule::updateListenToContainer(OSCQueryHelpers::OSCQueryValueContainer* gcc, bool onlySendIfListen)
{
	if (!enabled->boolValue() || !hasListenExtension || isCurrentlyLoadingData || isUpdatingStructure) return;
	if (wsClient == nullptr || !wsClient->isConnected)
	{
		NLOGWARNING(niceName, "Websocket not connected, can't LISTEN");
		return;
	}

	if (onlySendIfListen && !gcc->enableListen->boolValue()) return;

	String command = gcc->enableListen->boolValue() ? "LISTEN" : "IGNORE";
	Array<WeakReference<Controllable>> params = gcc->getAllControllables();

	var o(new DynamicObject());
	o.getDynamicObject()->setProperty("COMMAND", command);

	for (auto& p : params)
	{
		if (p == gcc->enableListen || p == gcc->syncContent) continue;
		String addr = p->getControlAddress(&valuesCC);
		o.getDynamicObject()->setProperty("DATA", addr);
		wsClient->send(JSON::toString(o, true));
	}

}


void GenericOSCQueryModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == useLocal)
	{
		remoteHost->setEnabled(!useLocal->boolValue());
	}
	else if (c == enabled || c == syncTrigger || c == remoteHost || c == remotePort || c == useAddressForNaming)
	{
		syncData();
	}
	else if (cc == &valuesCC)
	{
		if (OSCQueryHelpers::OSCQueryValueContainer* gcc = c->getParentAs<OSCQueryHelpers::OSCQueryValueContainer>())
		{
			if (c == gcc->enableListen)
			{
				updateListenToContainer(gcc);
			}
			else if (c == gcc->syncContent)
			{
				syncData();
			}
			else
			{
				sendOSCForControllable(c);
			}
		}
		else
		{
			sendOSCForControllable(c);
		}
	}
	else if (c == listenAllTrigger || c == listenNoneTrigger)
	{
		bool listenVal = c == listenAllTrigger;

		if (hasListenExtension)
		{
			Array<WeakReference<ControllableContainer>> containers = valuesCC.getAllContainers(true);
			for (auto& cc : containers)
			{
				if (OSCQueryHelpers::OSCQueryValueContainer* gcc = dynamic_cast<OSCQueryHelpers::OSCQueryValueContainer*>(cc.get())) gcc->enableListen->setValue(listenVal);
			}
		}
	}
}

void GenericOSCQueryModule::connectionOpened()
{
	NLOG(niceName, "Websocket connection is opened, let's get bi, baby !");
	isConnected->setValue(true);
	clearWarning("sync");
	updateAllListens();
}

void GenericOSCQueryModule::connectionClosed(int status, const String& reason)
{
	NLOG(niceName, "Websocket connection is closed, bye bye!");
	isConnected->setValue(false);
}

void GenericOSCQueryModule::connectionError(const String& errorMessage)
{
	NLOGERROR(niceName, "Connection error " << errorMessage);
	isConnected->setValue(false);
}

void GenericOSCQueryModule::dataReceived(const MemoryBlock& data)
{
	if (!enabled->boolValue()) return;

	OSCPacketParser parser(data.getData(), (int)data.getSize());
	OSCMessage m = parser.readMessage();

	processOSCMessage(m);
}

void GenericOSCQueryModule::processOSCMessage(const OSCMessage& m)
{
	if (logIncomingData->boolValue())
	{
		String s = m.getAddressPattern().toString();
		for (auto& a : m) s += "\n" + OSCHelpers::getStringArg(a);
		NLOG(niceName, "Feedback received : " << s);
	}

	inActivityTrigger->trigger();

	if (Controllable* c = OSCHelpers::findControllable(&valuesCC, m))
	{
		noFeedbackList.add(c);
		OSCHelpers::handleControllableForOSCMessage(c, m);
		noFeedbackList.removeAllInstancesOf(c);
	}
}

void GenericOSCQueryModule::messageReceived(const String& message)
{
	if (!enabled->boolValue()) return;

	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Websocket message received : " << message);
	}

	inActivityTrigger->trigger();
}

var GenericOSCQueryModule::getJSONData()
{
	var data = Module::getJSONData();
	data.getDynamicObject()->setProperty("treeData", treeData);
	data.getDynamicObject()->setProperty("hasListenExtension", hasListenExtension);
	return data;
}

void GenericOSCQueryModule::loadJSONDataInternal(var data)
{
	updateTreeFromData(data.getProperty("treeData", var()));
	hasListenExtension = data.getProperty("hasListenExtension", false);
	Module::loadJSONDataInternal(data);
}

void GenericOSCQueryModule::afterLoadJSONDataInternal()
{
	Module::afterLoadJSONDataInternal();
	syncData();
}

void GenericOSCQueryModule::endLoadFile()
{
	Engine::mainEngine->removeEngineListener(this);
	syncData();
}

void GenericOSCQueryModule::timerCallback()
{
	if (hasListenExtension && !isConnected->boolValue()) setupWSClient();
}

void GenericOSCQueryModule::run()
{
	if (useLocal == nullptr || remoteHost == nullptr || remotePort == nullptr) return;

	wait(100); //safety
	requestHostInfo();
}

void GenericOSCQueryModule::requestHostInfo()
{
	isConnected->setValue(false);
	hasListenExtension = false;

	URL url("http://" + (useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue()) + ":" + String(remotePort->intValue()) + "?HOST_INFO");
	StringPairArray responseHeaders;
	int statusCode = 0;

	std::unique_ptr<InputStream> stream(url.createInputStream(
		URL::InputStreamOptions(URL::ParameterHandling::inAddress)
		.withConnectionTimeoutMs(2000)
		.withResponseHeaders(&responseHeaders)
		.withStatusCode(&statusCode)
	));

	bool success = false;

#if JUCE_WINDOWS
	if (statusCode != 200)
	{
		stream.reset();
	}
#endif

	if (stream != nullptr)
	{
		String content = stream->readEntireStreamAsString();
		if (logIncomingData->boolValue()) NLOG(niceName, "Request status code : " << statusCode << ", content :\n" << content);

		inActivityTrigger->trigger();

		var data = JSON::parse(content);
		if (data.isObject())
		{
			if (logIncomingData->boolValue()) NLOG(niceName, "Received HOST_INFO :\n" << JSON::toString(data));

			success = true;

			String curServerName = serverName->stringValue();
			String newServerName = data.getProperty("NAME", "");
			if (curServerName.isNotEmpty() && newServerName != curServerName)
			{
				if (onlySyncSameName->boolValue())
				{
					NLOGWARNING(niceName, "Server name is not the same (" << curServerName << " <> " << newServerName << "), not syncing");
					return;
				}
				else
				{
					NLOG(niceName, "Server name has changed, if you wish to not sync, please enable \"Only sync from same name\"");
					serverName->setValue(newServerName);
				}
			}
			else
			{
				serverName->setValue(newServerName);
			}


			//String oscIP = data.getProperty("OSC_IP", remoteHost->stringValue());
			int oscPort = data.getProperty("OSC_PORT", remotePort->intValue());
			remoteOSCPort->setEnabled(oscPort != remotePort->intValue());
			remoteOSCPort->setValue(oscPort);
			if (oscPort != remotePort->intValue()) NLOG(niceName, "OSC_PORT is different from OSCQuery port, setting custom OSC port to " << oscPort);

			int wsPort = data.getProperty("WS_PORT", remotePort->intValue());
			remoteWSPort->setEnabled(wsPort != remotePort->intValue());
			remoteWSPort->setValue(wsPort);
			if (wsPort != remotePort->intValue()) NLOG(niceName, "WS_PORT is different from OSCQuery port, setting custom Websocket port to " << wsPort);


			hasListenExtension = data.getProperty("EXTENSIONS", var()).getProperty("LISTEN", false);
			requestStructure();

			if (hasListenExtension) NLOG(niceName, "Server has LISTEN extension, setting up websocket");
			setupWSClient();
		}

	}

	if (!success)
	{
		setWarningMessage("Can't connect to OSCQuery server", "sync");

		if (getWarningMessage("sync").isEmpty()) NLOGERROR(niceName, "Error with host info request, status code : " << statusCode << ", url : " << url.toString(true));
	}
	else
	{
		clearWarning("sync");
	}
}

void GenericOSCQueryModule::requestStructure()
{
	URL url("http://" + (useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue()) + ":" + String(remotePort->intValue()));
	StringPairArray responseHeaders;
	int statusCode = 0;

	std::unique_ptr<InputStream> stream(url.createInputStream(
		URL::InputStreamOptions(URL::ParameterHandling::inAddress)
		.withConnectionTimeoutMs(15000)
		.withResponseHeaders(&responseHeaders)
		.withStatusCode(&statusCode)
	));

#if JUCE_WINDOWS
	if (statusCode != 200)
	{
		NLOGWARNING(niceName, "Failed to request Structure, status code = " + String(statusCode));
		return;
	}
#endif


	if (stream != nullptr)
	{
		String content = stream->readEntireStreamAsString();
		if (logIncomingData->boolValue()) NLOG(niceName, "Request status code : " << statusCode << ", content :\n" << content);

		inActivityTrigger->trigger();

		var data = JSON::parse(content);
		if (data.isObject())
		{
			//if (logIncomingData->boolValue()) NLOG(niceName, "Received structure :\n" << JSON::toString(data));

			updateTreeFromData(data);

			Array<var> args;
			args.add(data);
			scriptManager->callFunctionOnAllItems(dataStructureEventId, args);
		}
	}
	else
	{
		if (logIncomingData->boolValue()) NLOGWARNING(niceName, "Error with request, status code : " << statusCode << ", url : " << url.toString(true));
	}
}

void GenericOSCQueryModule::handleRoutedModuleValue(Controllable* c, RouteParams* p)
{
	if (!enabled->boolValue()) return;

	if (Parameter* sourceP = dynamic_cast<Parameter*>(c))
	{
		OSCQueryRouteParams* qp = (OSCQueryRouteParams*)p;
		if (qp->cRef.wasObjectDeleted() || qp->cRef == nullptr) return;

		if (Parameter* outP = dynamic_cast<Parameter*>(qp->cRef.get()))
		{
			if (outP->value.isArray() == sourceP->value.isArray())
			{
				outP->setValue(sourceP->value);
			}
		}
	}
}


OSCQueryOutput::OSCQueryOutput(GenericOSCQueryModule* module) :
	EnablingControllableContainer("Output"),
	module(module)
{

}

OSCQueryOutput::~OSCQueryOutput()
{
}

InspectableEditor* OSCQueryOutput::getEditorInternal(bool isRoot, Array<Inspectable*> inspectables)
{
	return new OSCQueryModuleOutputEditor(this, isRoot);
}

GenericOSCQueryModule::OSCQueryRouteParams::OSCQueryRouteParams(GenericOSCQueryModule* outModule, Module* sourceModule, Controllable* c)
{
	target = addTargetParameter("Target", "The target value to modify", &outModule->valuesCC);
	target->excludeTypesFilter.add(Trigger::getTypeStringStatic());
}

GenericOSCQueryModule::OSCQueryRouteParams::~OSCQueryRouteParams()
{
	setControllable(nullptr);
}

void GenericOSCQueryModule::OSCQueryRouteParams::setControllable(Controllable* c)
{
	if (!cRef.wasObjectDeleted() && cRef != nullptr)
	{
		cRef->removeInspectableListener(this);
	}

	cRef = c;

	if (cRef != nullptr)
	{
		cRef->addInspectableListener(this);
	}
}

void GenericOSCQueryModule::OSCQueryRouteParams::onContainerParameterChanged(Parameter* p)
{
	if (p == target) setControllable(target->target);
}

void GenericOSCQueryModule::OSCQueryRouteParams::inspectableDestroyed(Inspectable* i)
{
	if (i == cRef) setControllable(nullptr);
}
