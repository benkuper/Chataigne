/*
  ==============================================================================

    HTTPModule.h
    Created: 13 Feb 2019 11:28:20am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class HTTPModule :
	public Module,
	public Thread
{
public:
	HTTPModule(const String& name = "HTTP");
	~HTTPModule();

	enum ResultDataType { RAW, JSON, XML, DEFAULT };
	
	StringParameter * baseAddress;
	BoolParameter* autoAdd;
	EnumParameter* protocol;

	EnablingControllableContainer authenticationCC;
	StringParameter* username;
	StringParameter* pass;
	String authHeader;

	Trigger* clearValues;


	const Identifier sendGETId = "sendGET";
	const Identifier sendPOSTId = "sendPOST";
	const Identifier sendPUTId = "sendPUT";
	const Identifier sendPATCHId = "sendPATCH";
	const Identifier sendDELETEId = "sendDELETE";

	const Identifier jsonDataTypeId = "json";
	const Identifier rawDataTypeId = "raw";
	const Identifier dataEventId = "dataEvent";

	enum RequestMethod { GET, METHOD_POST, METHOD_PUT, METHOD_PATCH, METHOD_DELETE, TYPE_MAX};
	static const String requestMethodNames[TYPE_MAX];


	void sendRequest(StringRef address, RequestMethod method, ResultDataType dataType = ResultDataType::RAW, StringPairArray params = StringPairArray(), String extraHeaders = String());

	struct Request
	{
		Request(URL u, RequestMethod m, ResultDataType dataType = ResultDataType::DEFAULT, String extraHeaders = String()) : url(u), method(m), resultDataType(dataType), extraHeaders(extraHeaders){}

		URL url;
		RequestMethod method;
		ResultDataType resultDataType;
		String extraHeaders;
	};


	OwnedArray<Request, CriticalSection> requests;
	void processRequest(Request * request);

	void createControllablesFromJSONResult(var data, ControllableContainer* container);
	void createControllablesFromXMLResult(XmlElement * data, ControllableContainer* container);
	void onControllableFeedbackUpdateInternal(ControllableContainer*, Controllable* c) override;

	//Script
	void sendRequestFromScript(const var::NativeFunctionArgs& args, RequestMethod method);
	static var sendGETFromScript(const var::NativeFunctionArgs& args);
	static var sendPOSTFromScript(const var::NativeFunctionArgs& args);
	static var sendPUTFromScript(const var::NativeFunctionArgs& args);
	static var sendPATCHFromScript(const var::NativeFunctionArgs& args);
	static var sendDELETEFromScript(const var::NativeFunctionArgs& args);

	String getDefaultTypeString() const override { return "HTTP"; }
	static HTTPModule * create() { return new HTTPModule(); }

	// Inherited via Thread
	virtual void run() override;
};