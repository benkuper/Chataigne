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
	public SimpleWebSocketServerBase::Listener

{
public:
	WebSocketServerModule(const String& name = "WebSocket Server", int defaultRemotePort = 8080);
	virtual ~WebSocketServerModule();

	IntParameter* localPort;
	BoolParameter* useSecureConnection;

	IntParameter* numClients;
	BoolParameter* isConnected;

	std::unique_ptr<SimpleWebSocketServerBase> server;

	const Identifier wsMessageReceivedId = "wsMessageReceived";
	const Identifier wsDataReceivedId = "wsDataReceived";

	void setupServer();

	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String& message, var) override;
	virtual void sendBytesInternal(Array<uint8> data, var) override;

	void connectionOpened(const String &connectionId) override;
	void connectionClosed(const String &connectionId, int status, const String &reason) override;
	void connectionError(const String& connectionId, const String& errorMessage) override;

	virtual void messageReceived(const String& connectionId, const String& message) override;
	virtual void dataReceived(const String& connectionId, const MemoryBlock &data) override;


	void onContainerParameterChangedInternal(Parameter* p) override;
	void onControllableFeedbackUpdateInternal(ControllableContainer* cc, Controllable* c) override;

	void createControllablesFromJSONResult(var data, ControllableContainer* container);

	void afterLoadJSONDataInternal() override;

	ModuleUI* getModuleUI() override;

	static WebSocketServerModule* create() { return new WebSocketServerModule(); }
	virtual String getDefaultTypeString() const override { return "WebSocket Server"; }
};