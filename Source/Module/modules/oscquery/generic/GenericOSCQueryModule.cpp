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
	Thread("OSCQuery")
{
	alwaysShowValues = true;
	setupIOConfiguration(false, true);

	syncTrigger = moduleParams.addTrigger("Sync Data", "Sync the data");

	sendCC = new OSCQueryOutput(this);
	moduleParams.addChildControllableContainer(sendCC);

	useLocal = sendCC->addBoolParameter("Local", "Send to Local IP (127.0.0.1). Allow to quickly switch between local and remote IP.", true);
	remoteHost = sendCC->addStringParameter("Remote Host", "Remote Host to send to.", "127.0.0.1");
	remoteHost->setEnabled(!useLocal->boolValue());
	remotePort = sendCC->addIntParameter("Remote port", "Port on which the remote host is listening to", defaultRemotePort, 1, 65535);

	defManager.add(CommandDefinition::createDef(this, "", "Set Value", &GenericOSCQueryCommand::create, CommandContext::BOTH));

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

	sender.sendToIPAddress(remoteHost->stringValue(), remotePort->intValue(), m);
}

void GenericOSCQueryModule::sendOSCForControllable(Controllable * c)
{
	if (!enabled->boolValue()) return;

	OSCMessage m(c->getControlAddress(&valuesCC));
	if (c->type != Controllable::TRIGGER) m.addArgument(OSCHelpers::varToArgument(((Parameter *)c)->value));
	sendOSCMessage(m);
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
			int access = nv.value.getProperty("ACCESS", -1);
			if (access == 0) //group
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
			else if (access > 0) //parameter
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

	if (c == syncTrigger || c == remoteHost || c == remotePort)
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
	URL url("http://" + (useLocal->boolValue()?"127.0.0.1":remoteHost->stringValue()) + ":" + remotePort->stringValue());

	StringPairArray responseHeaders;
	int statusCode = 0;
	ScopedPointer<InputStream> stream(url.createInputStream(false, nullptr, nullptr, String(),
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
