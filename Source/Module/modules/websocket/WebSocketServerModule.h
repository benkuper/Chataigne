/*
  ==============================================================================

    WebSocketServerModule.h
    Created: 20 Jun 2020 3:10:02pm
    Author:  bkupe

  ==============================================================================
*/

#pragma once

class WebSocketServerModule :
	public StreamingModule,
	public SimpleWebSocketServer::Listener

{
public:
	WebSocketServerModule(const String& name = "WebSocket Server", int defaultRemotePort = 8080);
	virtual ~WebSocketServerModule();

	IntParameter* localPort;
	IntParameter* numClients;
	BoolParameter* isConnected;

	std::unique_ptr<SimpleWebSocketServer> server;

	void setupServer();

	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String& message, var) override;
	virtual void sendBytesInternal(Array<uint8> data, var) override;

	void connectionOpened(const String &connectionId) override;
	void connectionClosed(const String &connectionId, int status, const String &reason) override;
	void connectionError(const String& connectionId, const String& errorMessage) override;

	void messageReceived(const String& connectionId, const String& message) override;
	

	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void createControllablesFromJSONResult(var data, ControllableContainer* container);

	ModuleUI* getModuleUI() override;

	static WebSocketServerModule* create() { return new WebSocketServerModule(); }
	virtual String getDefaultTypeString() const override { return "WebSocket Server"; }
};