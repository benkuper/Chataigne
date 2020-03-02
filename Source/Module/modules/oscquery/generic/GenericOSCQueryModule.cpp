/*
  ==============================================================================

	GenericOSCQueryModule.cpp
	Created: 28 Feb 2019 10:33:17pm
	Author:  bkupe

  ==============================================================================
*/

#include "GenericOSCQueryModule.h"
#include "ui/OSCQueryModuleEditor.h"
#include "GenericOSCQueryCommand.h"

GenericOSCQueryModule::GenericOSCQueryModule(const String & name, int defaultRemotePort) :
	Module(name),
	Thread("OSCQuery"),
    useLocal(nullptr),
    remoteHost(nullptr),
    remotePort(nullptr)
{
	alwaysShowValues = true;
	setupIOConfiguration(false, true);

	syncTrigger = moduleParams.addTrigger("Sync Data", "Sync the data");

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

	defManager->add(CommandDefinition::createDef(this, "", "Set Value", &GenericOSCQueryCommand::create, CommandContext::BOTH));

	sender.connect("0.0.0.0", 0);

	syncTrigger->trigger();
}

GenericOSCQueryModule::~GenericOSCQueryModule()
{
	signalThreadShouldExit();
	waitForThreadToExit(2000);
}

void GenericOSCQueryModule::sendOSCMessage(OSCMessage m)
{
	if (!enabled->boolValue()) return;

	if (logOutgoingData->boolValue())
	{
		NLOG(niceName, "Send OSC : " << m.getAddressPattern().toString());
		for (auto &a : m) LOG(OSCHelpers::getStringArg(a));
	}

	outActivityTrigger->trigger();

	sender.sendToIPAddress(remoteHost->stringValue(), remoteOSCPort->enabled?remoteOSCPort->intValue():remotePort->intValue(), m);
}

void GenericOSCQueryModule::sendOSCForControllable(Controllable * c)
{
	if (!enabled->boolValue()) return;

	String s = c->getControlAddress(&valuesCC);
	try
	{
		OSCMessage m(s);
		if (c->type != Controllable::TRIGGER) m.addArgument(OSCHelpers::varToArgument(((Parameter *)c)->value));
		sendOSCMessage(m);
	}
	catch (OSCFormatError& e)
	{
		NLOGERROR(niceName, "Can't send to address " << s << " : " << e.description);
	}

}


var GenericOSCQueryModule::sendOSCFromScript(const var::NativeFunctionArgs & a)
{
	GenericOSCQueryModule * m = getObjectFromJS<GenericOSCQueryModule>(a);
	if (!m->enabled->boolValue()) return var();

	if (a.numArguments == 0) return var();

	try
	{
		OSCMessage msg(a.arguments[0].toString());

		for (int i = 1; i < a.numArguments; i++)
		{
			if (a.arguments[i].isArray())
			{
				Array<var> * arr = a.arguments[i].getArray();
				for (auto &aa : *arr) msg.addArgument(varToArgument(aa));
			}
			else
			{
				msg.addArgument(varToArgument(a.arguments[i]));
			}
		}

		m->sendOSCMessage(msg);
	}
	catch (OSCFormatError &e)
	{
		NLOGERROR(m->niceName, "Error sending message : " << e.description);
	}
	

	return var();
}


OSCArgument GenericOSCQueryModule::varToArgument(const var & v)
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
	startThread();
}

void GenericOSCQueryModule::createTreeFromData(var data)
{
	valuesCC.clear();
	fillContainerFromData(&valuesCC, data);
}

void GenericOSCQueryModule::fillContainerFromData(ControllableContainer * cc, var data)
{
	DynamicObject * dataObject = data.getProperty("CONTENTS", var()).getDynamicObject();
	if (dataObject != nullptr)
	{
		NamedValueSet nvSet = dataObject->getProperties(); 
		for (auto & nv : nvSet)
		{
			//int access = nv.value.getProperty("ACCESS", 1);
			bool isGroup = /*access == 0 || */nv.value.hasProperty("CONTENTS");
			if (isGroup) //group
			{
				String ccNiceName = nv.value.getProperty("DESCRIPTION", "");
				if (ccNiceName.isEmpty()) ccNiceName = nv.name.toString();
				
				ControllableContainer * childCC = new ControllableContainer(ccNiceName);
				childCC->setCustomShortName(nv.name.toString());
				fillContainerFromData(childCC, nv.value);
				valuesContainers.add(childCC);
				childCC->editorIsCollapsed = true;

				cc->addChildControllableContainer(childCC);
			}
			else
			{
				Controllable * c = createControllableFromData(nv.name, nv.value);
				if(c != nullptr) cc->addControllable(c);
			}
		}
	}
}

Controllable * GenericOSCQueryModule::createControllableFromData(StringRef name, var data)
{
	Controllable * c = nullptr;

	String cNiceName = data.getProperty("DESCRIPTION", "");
	if (cNiceName.isEmpty()) cNiceName = name;

	const char type = data.getProperty("TYPE", "").toString()[0];
	var range = data.hasProperty("RANGE") ? data.getProperty("RANGE", var())[0] : var();
	var value = data.getProperty("VALUE", 0);
	if(value.isArray()) value = value[0];

	int access = data.getProperty("ACCESS", 3);

	var minVal = range.getProperty("MIN", INT32_MIN);
	var maxVal = range.getProperty("MAX", INT32_MAX);

	switch (type)
	{
	case 'N': c = new Trigger(cNiceName, cNiceName); break;
	case 'i': c = new IntParameter(cNiceName, cNiceName, value, minVal, maxVal); break;
	case 'f': c = new FloatParameter(cNiceName, cNiceName, value, minVal, maxVal); break;

	case 's':
	{
		if (range.isObject()) //enum
		{
			var options = range.getProperty("VALS",var());
			
			DBG(JSON::toString(range));

			if (options.isArray())
			{
				EnumParameter * ep = new EnumParameter(cNiceName, cNiceName);
				for (int i = 0; i < options.size(); i++) ep->addOption(options[i], options[i], false);
				ep->setValue(value);

				c = ep;
			}
		}
		else
		{
			c =  new StringParameter(cNiceName, cNiceName, value);
		}
	}
	break;

	case 'r':
	{
		Colour col = Colour::fromString(value.toString());
		Colour goodCol = Colour(col.getAlpha(), col.getRed(), col.getGreen(), col.getBlue()); //inverse RGBA > ARGB
		c = new ColorParameter(cNiceName, cNiceName, goodCol);
	}
	break;
	}

	if (c != nullptr)
	{
		c->setCustomShortName(name);
		if (access == 1) c->setControllableFeedbackOnly(true);
	
	}

	
	return c;
}

void GenericOSCQueryModule::onControllableFeedbackUpdateInternal(ControllableContainer * cc, Controllable * c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == useLocal)
	{
		remoteHost->setEnabled(!useLocal->boolValue());
	}else if (c == syncTrigger || c == remoteHost || c == remotePort)
	{
		syncData();
	}
	else if (cc == &valuesCC)
	{
		sendOSCForControllable(c);
	}
}

void GenericOSCQueryModule::run()
{
    if(useLocal == nullptr || remoteHost == nullptr || remotePort == nullptr) return;
	URL url("http://" + (useLocal->boolValue()?"127.0.0.1":remoteHost->stringValue()) + ":" + remotePort->stringValue());

	StringPairArray responseHeaders;
	int statusCode = 0;
	std::unique_ptr<InputStream> stream(url.createInputStream(false, nullptr, nullptr, String(),
		2000, // timeout in millisecs
		&responseHeaders, &statusCode));
#if JUCE_WINDOWS
	if (statusCode != 200)
	{
		NLOGWARNING(niceName, "Failed to connect, status code = " + String(statusCode));
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
			createTreeFromData(data);

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


OSCQueryOutput::OSCQueryOutput(GenericOSCQueryModule * module) :
	EnablingControllableContainer("Output"),
	module(module)
{
	
}

OSCQueryOutput::~OSCQueryOutput()
{
}

InspectableEditor * OSCQueryOutput::getEditor(bool isRoot)
{
	return new OSCQueryModuleOutputEditor(this, isRoot);
}
