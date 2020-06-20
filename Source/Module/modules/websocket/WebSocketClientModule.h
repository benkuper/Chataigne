/*
  ==============================================================================

    WebSocketClientModule.h
    Created: 20 Jun 2020 3:09:11pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


#pragma once

#include "../common/streaming/NetworkStreamingModule.h"

class WebSocketClientModule :
	public StreamingModule,
	public SimpleWebSocketClient::Listener

{
public:
	WebSocketClientModule(const String& name = "WebSocket Client", const String &defaultServerPath = "127.0.0.1:8080/ws");
	virtual ~WebSocketClientModule();

	StringParameter* serverPath;
	BoolParameter* isConnected;

	std::unique_ptr<SimpleWebSocketClient> client;

	void setupClient();

	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String& message, var) override;
	virtual void sendBytesInternal(Array<uint8> data, var) override;

	void connectionOpened() override;
	void connectionClosed(int status, const String& reason) override;
	void connectionError(const String& errorMessage) override;

	void messageReceived(const String& message) override;


	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;


	ModuleUI* getModuleUI() override;

	static WebSocketClientModule* create() { return new WebSocketClientModule(); }
	virtual String getDefaultTypeString() const override { return "WebSocket Client"; }
};