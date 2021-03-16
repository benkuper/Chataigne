/*
  ==============================================================================

    WebSocketClientModule.h
    Created: 20 Jun 2020 3:09:11pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once


#pragma once

class WebSocketClientModule :
	public StreamingModule,
	public SimpleWebSocketClient::Listener,
	public Timer

{
public:
	WebSocketClientModule(const String& name = "WebSocket Client", const String &defaultServerPath = "127.0.0.1:8080/ws");
	virtual ~WebSocketClientModule();

	StringParameter* serverPath;
	BoolParameter* isConnected;
	bool connectFirstTry;

	std::unique_ptr<SimpleWebSocketClient> client;

	const Identifier wsMessageReceivedId = "wsMessageReceived";
	const Identifier wsDataReceivedId = "wsDataReceived";

	virtual void setupClient();

	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String& message, var) override;
	virtual void sendBytesInternal(Array<uint8> data, var) override;

	virtual void connectionOpened() override;
	virtual void connectionClosed(int status, const String& reason) override;
	virtual void connectionError(const String& errorMessage) override;

	virtual void messageReceived(const String& message) override;
	virtual void dataReceived(const MemoryBlock& data) override;

	virtual void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	virtual void timerCallback() override;

	ModuleUI* getModuleUI() override;

	static WebSocketClientModule* create() { return new WebSocketClientModule(); }
	virtual String getDefaultTypeString() const override { return "WebSocket Client"; }
};