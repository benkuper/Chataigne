/*
  ==============================================================================

    HTTPModule.h
    Created: 13 Feb 2019 11:28:20am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../Module.h"

class HTTPModule :
	public Module,
	public Thread
{
public:
	HTTPModule(const String& name = "HTTP");
	~HTTPModule();

	StringParameter * baseAddress;
	BoolParameter* autoAdd;
	Trigger* clearValues;

	const Identifier sendGETId = "sendGET";
	const Identifier sendPOSTId = "sendPOST";
	const Identifier jsonDataTypeId = "json";
	const Identifier rawDataTypeId = "raw";
	const Identifier dataEventId = "dataEvent";

	enum RequestMethod { GET, POST };
	enum ResultDataType { RAW, JSON };


	void sendRequest(StringRef address, RequestMethod method, ResultDataType dataType = ResultDataType::RAW, StringPairArray params = StringPairArray());

	struct Request
	{
		Request(URL u, RequestMethod m, ResultDataType dataType = ResultDataType::RAW) : url(u), method(m), resultDataType(dataType) {}

		URL url;
		RequestMethod method;
		ResultDataType resultDataType;
	};


	OwnedArray<Request, CriticalSection> requests;
	void processRequest(Request * request);

	void createControllablesFromJSONResult(var data, ControllableContainer* container);
	void onControllableFeedbackUpdateInternal(ControllableContainer*, Controllable* c) override;

	//Script
	void sendRequestFromScript(const var::NativeFunctionArgs& args, RequestMethod method);
	static var sendGETFromScript(const var::NativeFunctionArgs& args);
	static var sendPOSTFromScript(const var::NativeFunctionArgs& args);

	String getDefaultTypeString() const override { return "HTTP"; }
	static HTTPModule * create() { return new HTTPModule(); }

	// Inherited via Thread
	virtual void run() override;
};