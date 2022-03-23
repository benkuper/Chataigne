#include "HTTPModule.h"
/*
  ==============================================================================

	HTTPModule.cpp
	Created: 13 Feb 2019 11:28:20am
	Author:  bkupe

  ==============================================================================
*/

const String HTTPModule::requestMethodNames[TYPE_MAX]{ "GET", "POST","PUT", "PATCH", "DELETE" };

HTTPModule::HTTPModule(const String& name) :
	Module(name),
	Thread("HTTPModule Requests"),
	authenticationCC("Authentication")
{
	includeValuesInSave = true;

	baseAddress = moduleParams.addStringParameter("Base Address", "The base adress to prepend to command addresses", "https://httpbin.org/");
	autoAdd = moduleParams.addBoolParameter("Auto add", "If checked, will try to add values depending on received data and expected data type", true);
	protocol = moduleParams.addEnumParameter("Protocol", "The type of content to expect when receiving data");
	protocol->addOption("Raw", RAW)->addOption("JSON", JSON)->addOption("XML", XML);

	username = authenticationCC.addStringParameter("Username", "If using authentication, this is the username to use for the authentication", "");
	pass = authenticationCC.addStringParameter("Password", "If using authentication, this is the password to use for the authentication", "");
	authenticationCC.enabled->setValue(false);
	moduleParams.addChildControllableContainer(&authenticationCC);

	clearValues = moduleParams.addTrigger("Clear values", "When triggered, this will remove all stored values in this module");

	valuesCC.userCanAddControllables = true;
	valuesCC.saveAndLoadRecursiveData = true;

	defManager->add(CommandDefinition::createDef(this, "", "Request", &HTTPCommand::create, CommandContext::BOTH));
	defManager->add(CommandDefinition::createDef(this, "", "Request with payload", &HTTPCommand::create, CommandContext::BOTH)->addParam("contentType", HTTPCommand::PLAIN));

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
	stopThread(3000);
}

void HTTPModule::sendRequest(StringRef address, RequestMethod method, ResultDataType dataType, StringPairArray params, String extraHeaders, String payload)
{

	String urlString = baseAddress->stringValue() + address;
	URL url = URL(urlString).withPOSTData(payload).withParameters(params);

	if (authHeader.isNotEmpty()) extraHeaders += "\r\n" + authHeader;

	outActivityTrigger->trigger();
	if (logOutgoingData->boolValue())  NLOG(niceName, "Send " + requestMethodNames[(int)method] + " Request : " + url.toString(true));

	requests.getLock().enter();
	requests.add(new Request(url, method, dataType, extraHeaders));
	requests.getLock().exit();
}

void HTTPModule::processRequest(Request* request)
{
	GenericScopedLock rLock(requests.getLock());

	StringPairArray responseHeaders;
	int statusCode = 0;

	std::unique_ptr<InputStream> stream(request->url.createInputStream(
		URL::InputStreamOptions(request->method == METHOD_POST ? URL::ParameterHandling::inPostData : URL::ParameterHandling::inAddress)
		.withConnectionTimeoutMs(2000)
		.withExtraHeaders(request->extraHeaders)
		.withResponseHeaders(&responseHeaders)
		.withStatusCode(&statusCode)
		.withNumRedirectsToFollow(5)
		.withProgressCallback(std::bind(&HTTPModule::requestProgressCallback, this, std::placeholders::_1, std::placeholders::_2))
		.withHttpRequestCmd(requestMethodNames[(int)request->method])
	));

#if JUCE_WINDOWS
	if (statusCode != 200 && !request->url.isLocalFile())
	{
		NLOGWARNING(niceName, "Failed to connect to " << request->url.toString(true) << ", status code = " << String(statusCode));
	}
#endif

	if (stream != nullptr)
	{
		String content = stream->readEntireStreamAsString();
		if (logIncomingData->boolValue()) NLOG(niceName, "Request status code : " << statusCode << ", content :\n" << content);

		inActivityTrigger->trigger();
		Array<var> args;

		ResultDataType rt = request->resultDataType == DEFAULT ? protocol->getValueDataAsEnum<ResultDataType>() : request->resultDataType;

		switch (rt)
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

		case XML:
		{
			if (autoAdd->boolValue())
			{
				std::unique_ptr<XmlElement> doc = XmlDocument::parse(content);
				if (doc != nullptr)
				{
					createControllablesFromXMLResult(doc.get(), &valuesCC);
				}
				else
				{
					NLOGERROR(niceName, "Content is not legit XML !");
				}
			}

			args.add(content);
		}

		break;

		default:
			break;
		}

		args.add(request->url.toString(true));
		scriptManager->callFunctionOnAllItems(dataEventId, args);
	}
	else
	{
		if (logIncomingData->boolValue()) NLOGWARNING(niceName, "Error with request, status code : " << statusCode << ", url : " << request->url.toString(true));
	}
}

bool HTTPModule::requestProgressCallback(int byteDownloaded, int bytesTotal)
{
	return !threadShouldExit();
}


void HTTPModule::createControllablesFromJSONResult(var data, ControllableContainer* container)
{
	if (!data.isObject()) return;

	DynamicObject* dataObject = data.getDynamicObject();
	if (dataObject == nullptr) return;

	NamedValueSet props = dataObject->getProperties();

	for (auto& p : props)
	{
		if (!p.name.isValid()) continue;

		if (p.value.isArray() && p.value.size() > 0 && p.value[0].isObject())
		{
			for (int i = 0; i < p.value.size(); ++i)
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
		}else if(p.value.isObject() && !p.value.isArray())
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
			createControllableFromJSONObject(p.name.toString(), p.value, container);
		}
	}
}

void HTTPModule::createControllableFromJSONObject(const String& cName, var data, ControllableContainer* container)
{
	Controllable* newC = container->getControllableByName(cName, true);
	if (newC == nullptr)
	{
		if (data.isBool()) newC = new BoolParameter(cName, cName, false);
		else if (data.isDouble()) newC = new FloatParameter(cName, cName, 0);
		else if (data.isInt()) newC = new IntParameter(cName, cName, 0);
		else if (data.isString() || data.isVoid()) newC = new StringParameter(cName, cName, "");
		else if (data.isArray())
		{
			if (data.size() == 1) newC = new FloatParameter(cName, cName, 0);
			else if (data.size() == 2) newC = new Point2DParameter(cName, cName);
			else if (data.size() == 3) newC = new Point3DParameter(cName, cName);
			else if (data.size() == 3) newC = new ColorParameter(cName, cName);
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
		if (newC->type == Controllable::TRIGGER && (int)data != 0) ((Trigger*)newC)->trigger();
		else
		{
			Parameter* param = dynamic_cast<Parameter*>(newC);
			if (param != nullptr) param->setValue(data.isVoid() ? "" : data, false, true);
		}
	}
}

void HTTPModule::createControllablesFromXMLResult(XmlElement* data, ControllableContainer* container)
{
	if (data == nullptr) return;

	int numChildren = data->getNumChildElements();
	for (int i = 0; i < numChildren; i++)
	{
		XmlElement* e = data->getChildElement(i);
		String eName = e->getTagName();

		if (e->getNumChildElements() == 1 && e->getFirstChildElement()->isTextElement())
		{
			String t = e->getFirstChildElement()->getText();

			Parameter* p = dynamic_cast<Parameter*>(container->getControllableByName(eName, true));

			if (p == nullptr && autoAdd->boolValue())
			{

				if (t == "false" || t == "true")
				{
					p = new BoolParameter(eName, eName, false);
				}if (t.getFloatValue() == 0 && !t.containsChar('0'))
				{
					p = new StringParameter(eName, eName, "");
				}
				else
				{
					p = new FloatParameter(eName, eName, 0);
				}

				if (p != nullptr)
				{
					container->addParameter(p);
				}
			}

			if (p != nullptr)
			{
				switch (p->type)
				{
				case Parameter::BOOL: p->setValue(t == "true");
				case Parameter::FLOAT:
				case Parameter::INT:
					p->setValue(t.getFloatValue());
					break;
				case Parameter::STRING: p->setValue(t);

				default:
					break;
				}
			}
		}
		else
		{
			ControllableContainer* cc = container->getControllableContainerByName(eName, true);
			if (cc == nullptr && autoAdd->boolValue())
			{
				cc = new ControllableContainer(eName);
				container->addChildControllableContainer(cc, true);
			}
			if (cc != nullptr) createControllablesFromXMLResult(e, cc);
		}
	}
}


void HTTPModule::onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c)
{
	Module::onControllableFeedbackUpdateInternal(cc, c);

	if (c == clearValues)
	{
		valuesCC.clear();
	}
	else if (c == authenticationCC.enabled || c == username || c == pass)
	{
		authHeader = authenticationCC.enabled->boolValue() ? ("Authorization: Basic " + Base64::toBase64(username->stringValue() + ":" + pass->stringValue())) : "";
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

	ResultDataType dataType = ResultDataType::DEFAULT;

	String extraHeaders = "";
	String payload = "";
	StringPairArray requestParams;

	if (args.numArguments >= 2)
	{
		if (args.arguments[1].isObject())
		{
			var o = args.arguments[1];
			extraHeaders = o.getProperty("extraHeaders", extraHeaders);

			var pp = o.getProperty("payload", "");
			if (pp.isObject()) payload = JSON::toString(pp, true);
			else payload = pp.toString();

			String dataTypeString = o.getProperty("dataType", "").toString().toLowerCase();
			if (dataTypeString == jsonDataTypeId.toString()) dataType = ResultDataType::JSON;
			else if (dataTypeString == rawDataTypeId.toString()) dataType = ResultDataType::RAW;

			var argArray = o.getProperty("arguments", var());
			for (int i = 0; i < argArray.size(); i += 2)
			{
				if (i >= argArray.size() - 1) break;
				requestParams.set(argArray[i], argArray[i + 1]);
			}
		}
		else
		{
			if (method == GET)
			{
				if (args.numArguments >= 2)
				{
					String dataTypeString = args.arguments[1].toString().toLowerCase();
					if (dataTypeString == jsonDataTypeId.toString()) dataType = ResultDataType::JSON;
					else if (dataTypeString == rawDataTypeId.toString()) dataType = ResultDataType::RAW;
					if (args.numArguments >= 3) extraHeaders = args.arguments[2].toString();
					if (args.numArguments >= 4)
					{
						var pp = args.arguments[3];
						if (pp.isObject()) payload = JSON::toString(pp, true);
						else payload = pp.toString();
					}
				}
			}
			else
			{
				for (int i = 1; i < args.numArguments; i += 2)
				{
					if (i >= args.numArguments - 1) break;
					requestParams.set(args.arguments[i], args.arguments[i + 1]);
				}
			}
		}
	}


	sendRequest(args.arguments[0].toString(), method, dataType, requestParams, extraHeaders, payload);
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
	if (m != nullptr) m->sendRequestFromScript(args, METHOD_POST);
	return var();
}

var HTTPModule::sendPUTFromScript(const var::NativeFunctionArgs& args)
{
	HTTPModule* m = getObjectFromJS<HTTPModule>(args);
	if (m != nullptr) m->sendRequestFromScript(args, METHOD_PUT);
	return var();
}

var HTTPModule::sendPATCHFromScript(const var::NativeFunctionArgs& args)
{
	HTTPModule* m = getObjectFromJS<HTTPModule>(args);
	if (m != nullptr) m->sendRequestFromScript(args, METHOD_PATCH);
	return var();
}

var HTTPModule::sendDELETEFromScript(const var::NativeFunctionArgs& args)
{
	HTTPModule* m = getObjectFromJS<HTTPModule>(args);
	if (m != nullptr) m->sendRequestFromScript(args, METHOD_DELETE);
	return var();
}

void HTTPModule::run()
{
	while (!threadShouldExit())
	{
		requests.getLock().enter();

		for (auto& r : requests) processRequest(r);
		requests.clear();
		requests.getLock().exit();
		wait(10);
	}
}
