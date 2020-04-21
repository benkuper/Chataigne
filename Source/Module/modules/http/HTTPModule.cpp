/*
  ==============================================================================

	HTTPModule.cpp
	Created: 13 Feb 2019 11:28:20am
	Author:  bkupe

  ==============================================================================
*/

#include "HTTPModule.h"
#include "commands/HTTPCommand.h"
#include "UI/ChataigneAssetManager.h"

const String HTTPModule::requestMethodNames[TYPE_MAX] { "GET", "POST","PUT", "PATCH", "DELETE" };

HTTPModule::HTTPModule(const String &name) :
	Module(name),
	Thread("HTTPModule Requests")
{
	includeValuesInSave = true; 
	
	baseAddress = moduleParams.addStringParameter("Base Address", "The base adress to prepend to command addresses", "https://httpbin.org/");
	autoAdd = moduleParams.addBoolParameter("Auto add", "If checked, will try to add values depending on received data and expected data type", true);
	clearValues = moduleParams.addTrigger("Clear values", "When triggered, this will remove all stored values in this module");

	valuesCC.userCanAddControllables = true;
	valuesCC.saveAndLoadRecursiveData = true;

	defManager->add(CommandDefinition::createDef(this, "", "Request", &HTTPCommand::create, CommandContext::BOTH));
	
	scriptObject.setMethod(sendGETId, HTTPModule::sendGETFromScript);
	scriptObject.setMethod(sendPOSTId, HTTPModule::sendPOSTFromScript);
	scriptObject.setMethod(sendPUTId, HTTPModule::sendPUTFromScript);
	scriptObject.setMethod(sendPATCHId, HTTPModule::sendPATCHFromScript);
	scriptObject.setMethod(sendDELETEId, HTTPModule::sendDELETEFromScript);
	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("http");

	startThread();
}

HTTPModule::~HTTPModule()
{
	signalThreadShouldExit();
	waitForThreadToExit(3000);
}

void HTTPModule::sendRequest(StringRef address, RequestMethod method, ResultDataType dataType, StringPairArray params, String extraHeaders)
{

	String urlString = baseAddress->stringValue() + address;
	URL url = URL(urlString).withParameters(params);


	outActivityTrigger->trigger();
	if (logOutgoingData->boolValue())  NLOG(niceName, "Send " + requestMethodNames[(int)method] + " Request : " + url.toString(true));

	requests.getLock().enter();
	requests.add(new Request(url, method, dataType, extraHeaders));
	requests.getLock().exit();
}

void HTTPModule::processRequest(Request * request)
{
	StringPairArray responseHeaders;
	int statusCode = 0;

	std::unique_ptr<InputStream> stream(request->url.createInputStream(request->method == POST, nullptr, nullptr, request->extraHeaders,
		2000, // timeout in millisecs
		&responseHeaders, &statusCode, 5, requestMethodNames[(int)request->method]));

#if JUCE_WINDOWS
	if (statusCode != 200 && !request->url.isLocalFile())
	{
		NLOGWARNING(niceName, "Failed to connect to " << request->url.toString(true) << ", status code = " << String(statusCode));
		return;
	}
#endif

	if (stream != nullptr)
	{
		String content = stream->readEntireStreamAsString();
		if (logIncomingData->boolValue()) NLOG(niceName, "Request status code : " << statusCode << ", content :\n" << content);
		
		inActivityTrigger->trigger();
		Array<var> args;
		
		switch (request->resultDataType)
		{
		case RAW:
			args.add(content);
			break;

		case JSON:
		{
			var data = JSON::parse(content);
			if (data.isObject())
			{
				args.add(data);
				if (autoAdd->boolValue()) createControllablesFromJSONResult(data, &valuesCC);
			}
			else
			{
				args.add(content);
				NLOGERROR(niceName, "Error parsing JSON content, data is badly formatted");
			}
		}
		break;
		}

		args.add(request->url.toString(true));
		scriptManager->callFunctionOnAllItems(dataEventId, args);

		
	} else
	{
		if (logIncomingData->boolValue()) NLOGWARNING(niceName, "Error with request, status code : " << statusCode << ", url : " << request->url.toString(true));
	}
}


void HTTPModule::createControllablesFromJSONResult(var data, ControllableContainer* container)
{
	if (!data.isObject()) return;

	DynamicObject* dataObject = data.getDynamicObject();
	if (dataObject == nullptr) return;

	NamedValueSet props = dataObject->getProperties();

	for (auto& p : props)
	{
		if (p.value.isArray())
		{
			for (int i = 0; i < p.value.size(); i++)
			{
				ControllableContainer* cc = container->getControllableContainerByName(String(i), true);
				if (cc == nullptr)
				{
					cc = new ControllableContainer(String(i));
					container->addChildControllableContainer(cc, true);
					cc->userCanAddControllables = true;
					cc->saveAndLoadRecursiveData = true;
					cc->saveAndLoadName = true;
				}

				createControllablesFromJSONResult(p.value[i], cc);
			}
			
		}else if (p.value.isObject())
		{
			ControllableContainer* cc = container->getControllableContainerByName(p.name.toString(), true);
			if (cc == nullptr)
			{
				cc = new ControllableContainer(p.name.toString());
				container->addChildControllableContainer(cc, true);
				cc->userCanAddControllables = true;
				cc->saveAndLoadRecursiveData = true;
				cc->saveAndLoadName = true;
			}

			createControllablesFromJSONResult(p.value, cc);
		}
		else
		{
			Controllable * newC = container->getControllableByName(p.name.toString(), true);
			if (newC == nullptr)
			{
				if (p.value.isBool()) newC = new BoolParameter(p.name.toString(), p.name.toString(), false);
				else if (p.value.isDouble()) newC = new FloatParameter(p.name.toString(), p.name.toString(), 0);
				else if (p.value.isInt()) newC = new IntParameter(p.name.toString(), p.name.toString(), 0);
				else if (p.value.isString() || p.value.isVoid()) newC = new StringParameter(p.name.toString(), p.name.toString(), "");
				else if (p.value.isArray())
				{
					if (p.value.size() == 1) newC = new FloatParameter(p.name.toString(), p.name.toString(), 0);
					else if (p.value.size() == 2) newC = new Point2DParameter(p.name.toString(), p.name.toString());
					else if (p.value.size() == 3) newC = new Point3DParameter(p.name.toString(), p.name.toString());
					else if (p.value.size() == 3) newC = new ColorParameter(p.name.toString(), p.name.toString());
				}


				if (newC != nullptr)
				{
					newC->isCustomizableByUser = true;
					newC->isRemovableByUser = true;
					newC->isSavable = true;
					newC->saveValueOnly = false;
					container->addControllable(newC);
				}
			}

			if (newC != nullptr)
			{
				if (newC->type == Controllable::TRIGGER && (int)p.value != 0) ((Trigger*)newC)->trigger();
				else
				{
					Parameter* param = dynamic_cast<Parameter*>(newC);
					if (param != nullptr) param->setValue(p.value.isVoid()?"":p.value, false, true);
				}
			}
		}
	}
}


void HTTPModule::onControllableFeedbackUpdateInternal(ControllableContainer*, Controllable* c)
{
	if (c == clearValues)
	{
		valuesCC.clear();
	}
}

void HTTPModule::sendRequestFromScript(const var::NativeFunctionArgs& args, RequestMethod method)
{
	if (!enabled->boolValue()) return;

	if (args.numArguments < 1)
	{
		LOGWARNING("Sending HTTP Request requires at least one argument being the address !");
		return;
	}

	ResultDataType dataType = ResultDataType::RAW;

	String extraHeaders = "";
	StringPairArray requestParams;

	if (method == GET)
	{
		if (args.numArguments >= 2)
		{
			String dataTypeString = args.arguments[1].toString().toLowerCase();
			if (dataTypeString == jsonDataTypeId.toString()) dataType = ResultDataType::JSON;
			else if (dataTypeString == rawDataTypeId.toString()) dataType = ResultDataType::RAW;
			if (args.numArguments >= 3) extraHeaders = args.arguments[2].toString();
		}
	}
	else
	{
		for (int i = 2; i < args.numArguments; i += 2)
		{
			if (i >= args.numArguments - 1) break;
			requestParams.set(args.arguments[i], args.arguments[i + 1]);
		}
	}
	
	sendRequest(args.arguments[0].toString(), method, dataType, requestParams, extraHeaders);
	return;
}

var HTTPModule::sendGETFromScript(const var::NativeFunctionArgs& args)
{
	HTTPModule* m = getObjectFromJS<HTTPModule>(args);
	if (m != nullptr) m->sendRequestFromScript(args, GET);
	return var();
}

var HTTPModule::sendPOSTFromScript(const var::NativeFunctionArgs& args)
{
	HTTPModule* m = getObjectFromJS<HTTPModule>(args);
	if (m != nullptr) m->sendRequestFromScript(args, POST);
	return var();
}

var HTTPModule::sendPUTFromScript(const var::NativeFunctionArgs& args)
{
	HTTPModule* m = getObjectFromJS<HTTPModule>(args);
	if (m != nullptr) m->sendRequestFromScript(args, PUT);
	return var();
}

var HTTPModule::sendPATCHFromScript(const var::NativeFunctionArgs& args)
{
	HTTPModule* m = getObjectFromJS<HTTPModule>(args);
	if (m != nullptr) m->sendRequestFromScript(args, PATCH);
	return var();
}

var HTTPModule::sendDELETEFromScript(const var::NativeFunctionArgs& args)
{
	HTTPModule* m = getObjectFromJS<HTTPModule>(args);
	if (m != nullptr) m->sendRequestFromScript(args, DELETE);
	return var();
}

void HTTPModule::run()
{
	while (!threadShouldExit())
	{
		requests.getLock().enter();

		for (auto &r : requests) processRequest(r);
		requests.clear();
		requests.getLock().exit();
		sleep(10);
	}
}
