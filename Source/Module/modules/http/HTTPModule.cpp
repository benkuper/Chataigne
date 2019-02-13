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

HTTPModule::HTTPModule() :
	Module(getTypeString()),
	Thread("HTTPModule Requests")
{
	baseAddress = moduleParams.addStringParameter("Base Address", "The base adress to prepend to command addresses", "https://httpbin.org/");
	defManager.add(CommandDefinition::createDef(this, "", "Request", &HTTPCommand::create, CommandContext::BOTH));
	
	scriptManager->scriptTemplate += ChataigneAssetManager::getInstance()->getScriptTemplate("http");

	startThread();
}

HTTPModule::~HTTPModule()
{
	signalThreadShouldExit();
	waitForThreadToExit(3000);
}

void HTTPModule::sendRequest(StringRef address, RequestMethod method, StringPairArray params)
{

	String urlString = baseAddress->stringValue() + address;
	URL url = URL(urlString).withParameters(params);

	outActivityTrigger->trigger();
	if (logOutgoingData->boolValue())  NLOG(niceName, "Send " + String(method == GET?"GET":"POST") + " Request : " + url.toString(true));

	requests.getLock().enter();
	requests.add(new Request(url, method));
	requests.getLock().exit();
}

void HTTPModule::processRequest(Request * request)
{
	StringPairArray responseHeaders;
	int statusCode = 0;
	ScopedPointer<InputStream> stream(request->url.createInputStream(request->method == POST, nullptr, nullptr, String(),
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
		Array<var> args;
		args.add(content);
		args.add(request->url.toString(true));
		scriptManager->callFunctionOnAllItems(dataEventId, args);

		/*
		var data = JSON::parse(content);
		if (data.isObject())
		{

		}
		*/
	} else
	{
		if (logIncomingData->boolValue()) NLOGWARNING(niceName, "Error with request, status code : " << statusCode << ", url : " << request->url.toString(true));
	}
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
