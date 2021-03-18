#include "GenericOSCQueryModule.h"
/*
  ==============================================================================

	GenericOSCQueryModule.cpp
	Created: 28 Feb 2019 10:33:17pm
	Author:  bkupe

  ==============================================================================
*/

GenericOSCQueryModule::GenericOSCQueryModule(const String& name, int defaultRemotePort) :
	Module(name),
	Thread("OSCQuery"),
	useLocal(nullptr),
	remoteHost(nullptr),
	remotePort(nullptr),
	hasListenExtension(false)
{
	alwaysShowValues = true;
	canHandleRouteValues = true;

	includeValuesInSave = true;

	setupIOConfiguration(true, true);

	keepValuesOnSync = moduleParams.addBoolParameter("Keep Values On Sync", "If checked, this will force keeping the current values when syncing the OSCQuery remote data structure.", false);
	syncTrigger = moduleParams.addTrigger("Sync Data", "Sync the data");
	serverName = moduleParams.addStringParameter("Server Name", "The name of the OSCQuery server, if provided", "");
	serverName->setControllableFeedbackOnly(true);
	listenAllTrigger = moduleParams.addTrigger("Listen to all", "This will automatically enable listen to all containers");

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

	//Script
	scriptObject.setMethod("send", GenericOSCQueryModule::sendOSCFromScript);

	defManager->add(CommandDefinition::createDef(this, "", "Set Value", &GenericOSCQueryCommand::create)->addParam("action", GenericOSCQueryCommand::SET_VALUE));
	defManager->add(CommandDefinition::createDef(this, "", "Trigger", &GenericOSCQueryCommand::create)->addParam("action", GenericOSCQueryCommand::TRIGGER));

	sender.connect("0.0.0.0", 0);

}

GenericOSCQueryModule::~GenericOSCQueryModule()
{
	if (wsClient != nullptr) wsClient->stop();
	stopThread(2000);
	valuesCC.clear();
}

void GenericOSCQueryModule::setupWSClient()
{
	if (wsClient != nullptr) wsClient->stop();
	wsClient.reset();
	if (isCurrentlyLoadingData) return;

	if (!enabled->intValue() || !hasListenExtension) return;
	NLOG(niceName, "Server has LISTEN extension, setting up websocket");
	wsClient.reset(new SimpleWebSocketClient());
	wsClient->addWebSocketListener(this);
	wsClient->start(remoteHost->stringValue() + ":" + remotePort->stringValue() + "/");
}

void GenericOSCQueryModule::sendOSCMessage(OSCMessage m)
{
	if (!enabled->boolValue()) return;

	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Send OSC : " << m.getAddressPattern().toString());
		for (auto& a : m) LOG(OSCHelpers::getStringArg(a));
	}

	outActivityTrigger->trigger();

	sender.sendToIPAddress(remoteHost->stringValue(), remoteOSCPort->enabled ? remoteOSCPort->intValue() : remotePort->intValue(), m);
}

void GenericOSCQueryModule::sendOSCForControllable(Controllable* c)
{
	if (!enabled->boolValue()) return;
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
					m.addArgument(OSCHelpers::varToArgument(p->value[i]));
				}
			}
			else
			{
				m.addArgument(OSCHelpers::varToArgument(p->value));
			}
		}
		sendOSCMessage(m);
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

		m->sendOSCMessage(msg);
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
	if (isCurrentlyLoadingData) return;

	startThread();
}

void GenericOSCQueryModule::updateTreeFromData(var data)
{
	if (data.isVoid()) return;

	Array<String> enableListenContainers;
	Array<String> expandedContainers;
	Array<WeakReference<ControllableContainer>> containers = valuesCC.getAllContainers(true);

	if (!keepValuesOnSync->boolValue())
	{
		for (auto& cc : containers)
		{
			if (GenericOSCQueryValueContainer* gcc = dynamic_cast<GenericOSCQueryValueContainer*>(cc.get()))
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
	

	var vData;
	if (keepValuesOnSync->boolValue()) vData = valuesCC.getJSONData();

	//valuesCC.clear();

	updateContainerFromData(&valuesCC, data);

	if (keepValuesOnSync->boolValue())
	{
		if (!vData.isVoid()) valuesCC.loadJSONData(vData);
	}
	else
	{
		for (auto& addr : enableListenContainers)
		{
			if (GenericOSCQueryValueContainer* gcc = dynamic_cast<GenericOSCQueryValueContainer*>(valuesCC.getControllableContainerForAddress(addr)))
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

void GenericOSCQueryModule::updateContainerFromData(ControllableContainer* cc, var data)
{
	DynamicObject* dataObject = data.getProperty("CONTENTS", var()).getDynamicObject();

	Array<WeakReference<ControllableContainer>> containersToDelete = cc->getAllContainers();
	Array<WeakReference<Controllable>> controllablesToDelete = cc->getAllControllables();
	
	if (GenericOSCQueryValueContainer* vc = dynamic_cast<GenericOSCQueryValueContainer*>(cc)) controllablesToDelete.removeAllInstancesOf(vc->enableListen);

	if (dataObject != nullptr)
	{
		NamedValueSet nvSet = dataObject->getProperties();
		for (auto& nv : nvSet)
		{
			//int access = nv.value.getProperty("ACCESS", 1);
			bool isGroup = /*access == 0 || */nv.value.hasProperty("CONTENTS");
			if (isGroup) //group
			{
				String ccNiceName = nv.value.getProperty("DESCRIPTION", "");
				if (ccNiceName.isEmpty()) ccNiceName = nv.name.toString();

				GenericOSCQueryValueContainer* childCC = dynamic_cast<GenericOSCQueryValueContainer*>(cc->getControllableContainerByName(ccNiceName, true));

				if (childCC == nullptr)
				{
					childCC = new GenericOSCQueryValueContainer(ccNiceName);
					childCC->saveAndLoadRecursiveData = true;
					childCC->setCustomShortName(nv.name.toString());
					childCC->editorIsCollapsed = true;
				}
				else containersToDelete.removeAllInstancesOf(childCC);

				updateContainerFromData(childCC, nv.value);

				if (childCC->parentContainer != cc) cc->addChildControllableContainer(childCC, true);
			}
			else
			{
				Controllable* c = cc->getControllableByName(nv.name.toString());
				if (c != nullptr) controllablesToDelete.removeAllInstancesOf(c);
				createOrUpdateControllableFromData(cc, c, nv.name, nv.value);
			}
		}
	}

	for (auto& cd : controllablesToDelete) cc->removeControllable(cd);
	for (auto& ccd : containersToDelete) cc->removeChildControllableContainer(ccd);	
}

void GenericOSCQueryModule::createOrUpdateControllableFromData(ControllableContainer* parentCC, Controllable* sourceC, StringRef name, var data)
{
	Controllable* c = sourceC;

	String cNiceName = data.getProperty("DESCRIPTION", "");
	if (cNiceName.isEmpty()) cNiceName = name;

	String type = data.getProperty("TYPE", "").toString();
	var valRange = data.hasProperty("RANGE") ? data.getProperty("RANGE", var()) : var();
	var val = data.getProperty("VALUE", var());
	int access = data.getProperty("ACCESS", 3);

	var value;
	var range;

	if (val.isArray()) value = val;
	else value.append(val);
	if (valRange.isArray()) range = valRange;
	else range.append(valRange);

	if (range.size() != value.size())
	{
		//DBG("Not the same : " << range.size() << " / " << value.size() << "\n" << data.toString());
		//NLOGWARNING(niceName, "RANGE and VALUE fields don't have the same size, skipping : " << cNiceName);
	}
	var minVal;
	var maxVal;
	for (int i = 0; i < range.size(); ++i)
	{
		minVal.append(range[i].getProperty("MIN", INT32_MIN));
		maxVal.append(range[i].getProperty("MAX", INT32_MAX));
	}

	Controllable::Type targetType;
	if (type == "N" || type == "I") targetType = Controllable::TRIGGER;
	else if (type == "i" || type == "h") targetType = Controllable::INT;
	else if (type == "f" || type == "d") targetType = Controllable::FLOAT;
	else if (type == "ii" || type == "ff" || type == "hh" || type == "dd") targetType = Controllable::POINT2D;
	else if (type == "iii" || type == "fff" || type == "hhh" || type == "ddd") targetType = Controllable::POINT3D;
	else if (type == "ffff" || type == "dddd" || type == "iiii" || type == "hhhh" || type == "r") targetType = Controllable::COLOR;
	else if (type == "s" || type == "S" || type == "c")
	{
		if (range[0].isObject()) targetType = Controllable::ENUM;
		else targetType = Controllable::STRING;
	}
	else if (type == "T" || type == "F") targetType = Controllable::BOOL;


	if (c != nullptr && targetType != c->type)
	{
		parentCC->removeControllable(c);
		c = nullptr;
	}

	bool addToContainer = c == nullptr;

	switch (targetType)
	{
	case Controllable::TRIGGER:
	{
		if (c == nullptr) c = new Trigger(cNiceName, cNiceName);
	}
	break;

	case Controllable::INT:
	{
		if (c == nullptr) c = new IntParameter(cNiceName, cNiceName, value[0], minVal[0], maxVal[0]);
		else
		{
			((Parameter*)c)->setValue(value[0]);
			((Parameter*)c)->setRange(minVal[0], maxVal[0]);
		}
	}
	break;

	case Controllable::FLOAT:
	{
		if (c == nullptr) c = new FloatParameter(cNiceName, cNiceName, value[0], minVal[0], maxVal[0]);
		else
		{
			((Parameter*)c)->setValue(value[0]);
			((Parameter*)c)->setRange(minVal[0], maxVal[0]);
		}
	}
	break;

	case Controllable::POINT2D:
	{
		if (value.isVoid()) for (int i = 0; i < 2; ++i) value.append(0);
		if (c == nullptr) c = new Point2DParameter(cNiceName, cNiceName);
		if (value.size() >= 2) ((Point2DParameter*)c)->setValue(value);
		if (range.size() >= 2) ((Point2DParameter*)c)->setRange(minVal, maxVal);
	}
	break;

	case Controllable::POINT3D:
	{
		if (value.isVoid()) for (int i = 0; i < 3; ++i) value.append(0);
		if (c == nullptr) c = new Point3DParameter(cNiceName, cNiceName);
		if (value.size() >= 3) ((Point3DParameter*)c)->setValue(value);
		if (range.size() >= 3) ((Point3DParameter*)c)->setRange(minVal, maxVal);
	}
	break;

	case Controllable::COLOR:
	{
		Colour col = Colours::black;
		if (type == "ffff" || type == "dddd") col = value.size() >= 4 ? Colour::fromFloatRGBA(value[0], value[1], value[2], value[3]) : Colours::black;
		else if (type == "iiii" || type == "hhhh") col = value.size() >= 4 ? Colour::fromRGBA((int)value[0], (int)value[1], (int)value[2], (int)value[3]) : Colours::black;
		else if (type == "r") col = Colour::fromString(value[0].toString());

		if (c == nullptr)  c = new ColorParameter(cNiceName, cNiceName, col);
		else ((ColorParameter*)c)->setColor(col);
	}
	break;

	case Controllable::STRING:
	{
		if (c == nullptr) c = new StringParameter(cNiceName, cNiceName, value[0]);
		else ((StringParameter*)c)->setValue(value[0]);
	}
	break;

	case Controllable::ENUM:
	{
		var options = range[0].getProperty("VALS", var());
		if (options.isArray())
		{
			if (c == nullptr) c = new EnumParameter(cNiceName, cNiceName);

			for (int i = 0; i < options.size(); ++i) ((EnumParameter*)c)->addOption(options[i], options[i], false);
			((EnumParameter*)c)->setValueWithKey(value[0]);
		}
	}
	break;

	case Controllable::BOOL:
	{
		if (c == nullptr) c = new BoolParameter(cNiceName, cNiceName, value[0]);
		else ((BoolParameter*)c)->setValue(value[0]);
	}
	break;
	}

	if (c != nullptr)
	{
		c->setCustomShortName(name);
		if (access == 1) c->setControllableFeedbackOnly(true);
	}

	if (addToContainer) parentCC->addControllable(c);
}


void GenericOSCQueryModule::updateListenToContainer(GenericOSCQueryValueContainer* gcc)
{
	if (!enabled->boolValue() || !hasListenExtension || isCurrentlyLoadingData) return;
	if (wsClient == nullptr || !wsClient->isConnected)
	{
		NLOGWARNING(niceName, "Websocket not connected, can't LISTEN");
		return;
	}

	String command = gcc->enableListen->boolValue() ? "LISTEN" : "IGNORE";
	Array<WeakReference<Parameter>> params = gcc->getAllParameters();

	var o(new DynamicObject());
	o.getDynamicObject()->setProperty("COMMAND", command);

	for (auto& p : params)
	{
		if (p == gcc->enableListen) continue;
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
	else if (c == enabled || c == syncTrigger || c == remoteHost || c == remotePort)
	{
		syncData();
	}
	else if (cc == &valuesCC)
	{
		if (GenericOSCQueryValueContainer* gcc = c->getParentAs<GenericOSCQueryValueContainer>())
		{
			if (c == gcc->enableListen)
			{
				updateListenToContainer(gcc);
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
	else if (c == listenAllTrigger)
	{
		if (hasListenExtension)
		{
			Array<WeakReference<ControllableContainer>> containers = valuesCC.getAllContainers(true);
			for (auto& cc : containers)
			{
				if (GenericOSCQueryValueContainer* gcc = dynamic_cast<GenericOSCQueryValueContainer*>(cc.get())) gcc->enableListen->setValue(true);
			}
		}
	}
}

void GenericOSCQueryModule::connectionOpened()
{
	NLOG(niceName, "Websocket connection is opened, let's get bi, baby !");
}

void GenericOSCQueryModule::connectionClosed(int status, const String& reason)
{
	NLOG(niceName, "Websocket connection is closed, bye bye!");
}

void GenericOSCQueryModule::connectionError(const String& errorMessage)
{
	if (enabled->boolValue()) NLOGERROR(niceName, "Connection error " << errorMessage);
}

void GenericOSCQueryModule::dataReceived(const MemoryBlock& data)
{
	if (logIncomingData->boolValue())
	{
		NLOG(niceName, "Websocket data received : " << (int)data.getSize() << " bytes");
	}

	inActivityTrigger->trigger();

	OSCPacketParser parser(data.getData(), (int)data.getSize());
	OSCMessage m = parser.readMessage();
	if (m.isEmpty())
	{
		LOGERROR("Empty message");
		return;
	}

	if (Controllable* c = OSCHelpers::findControllable(&valuesCC, m))
	{
		noFeedbackList.add(c);
		OSCHelpers::handleControllableForOSCMessage(c, m);
		noFeedbackList.removeAllInstancesOf(c);
	}

}

void GenericOSCQueryModule::messageReceived(const String& message)
{
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
	return data;
}

void GenericOSCQueryModule::loadJSONDataInternal(var data)
{
	updateTreeFromData(data.getProperty("treeData", var()));
	Module::loadJSONDataInternal(data);
}

void GenericOSCQueryModule::afterLoadJSONDataInternal()
{
	Module::afterLoadJSONDataInternal();
	syncData();
}

void GenericOSCQueryModule::run()
{
	if (useLocal == nullptr || remoteHost == nullptr || remotePort == nullptr) return;

	wait(100); //safety

	requestHostInfo();
	requestStructure();

}

void GenericOSCQueryModule::requestHostInfo()
{
	URL url("http://" + (useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue()) + ":" + String(remotePort->intValue()) + "?HOST_INFO");
	StringPairArray responseHeaders;
	int statusCode = 0;
	std::unique_ptr<InputStream> stream(url.createInputStream(false, nullptr, nullptr, String(),
		2000, // timeout in millisecs
		&responseHeaders, &statusCode));
#if JUCE_WINDOWS
	if (statusCode != 200)
	{
		NLOGWARNING(niceName, "Failed to request HOST_INFO, status code = " + String(statusCode));
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
			if (logIncomingData->boolValue()) NLOG(niceName, "Received HOST_INFO :\n" << JSON::toString(data));

			serverName->setValue(data.getProperty("NAME", ""));

			int oscPort = data.getProperty("OSC_PORT", remotePort->intValue());
			if (oscPort != remotePort->intValue())
			{
				NLOG(niceName, "OSC_PORT is different from remotePort, setting custom OSC Port to " << oscPort);
				remoteOSCPort->setEnabled(true);
				remoteOSCPort->setValue(oscPort);
			}

			hasListenExtension = data.getProperty("EXTENSIONS", var()).getProperty("LISTEN", false);
			setupWSClient();
		}
	}
	else
	{
		if (logIncomingData->boolValue()) NLOGWARNING(niceName, "Error with host info request, status code : " << statusCode << ", url : " << url.toString(true));
	}
}

void GenericOSCQueryModule::requestStructure()
{
	URL url("http://" + (useLocal->boolValue() ? "127.0.0.1" : remoteHost->stringValue()) + ":" + String(remotePort->intValue()));
	StringPairArray responseHeaders;
	int statusCode = 0;
	std::unique_ptr<InputStream> stream(url.createInputStream(false, nullptr, nullptr, String(),
		2000, // timeout in millisecs
		&responseHeaders, &statusCode));
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

InspectableEditor* OSCQueryOutput::getEditor(bool isRoot)
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

GenericOSCQueryValueContainer::GenericOSCQueryValueContainer(const String& name) :
	ControllableContainer(name)
{
	enableListen = addBoolParameter("Listen", "This will activate listening to this container", false);
	enableListen->hideInEditor = true;
}

GenericOSCQueryValueContainer::~GenericOSCQueryValueContainer()
{
}

InspectableEditor* GenericOSCQueryValueContainer::getEditor(bool isRoot)
{
	return new GenericOSCQueryValueContainerEditor(this, isRoot);
}
