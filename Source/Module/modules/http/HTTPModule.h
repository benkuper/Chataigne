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
	HTTPModule();
	~HTTPModule();

	StringParameter * baseAddress;

	
	enum RequestMethod { GET, POST };
	void sendRequest(StringRef address, RequestMethod method, StringPairArray params = StringPairArray());

	struct Request
	{
		Request(URL u, RequestMethod m) : url(u), method(m) {}
		URL url;
		RequestMethod method;
	};

	const Identifier dataEventId = "dataReceived";

	OwnedArray<Request, CriticalSection> requests;
	void processRequest(Request * request);

	String getTypeString() const override { return "HTTP"; }
	static HTTPModule * create() { return new HTTPModule(); }

	// Inherited via Thread
	virtual void run() override;
};