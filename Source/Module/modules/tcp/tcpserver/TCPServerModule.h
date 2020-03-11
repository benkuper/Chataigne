/*
  ==============================================================================

    TCPServerModule.h
    Created: 4 Jul 2019 10:39:27am
    Author:  bkupe

  ==============================================================================
*/

#pragma once

#include "../../common/streaming/NetworkStreamingModule.h"
#include "TCPServerConnectionManager.h"

class TCPServerModule :
	public NetworkStreamingModule,
	public TCPServerConnectionManager::ConnectionManagerListener

{
public:
	TCPServerModule(const String& name = "TCP Server", int defaultRemotePort = 5001);
	virtual ~TCPServerModule();


	TCPServerConnectionManager connectionManager;
	IntParameter* numClients;


	virtual void setupReceiver() override;
	virtual void initThread() override;
	virtual void clearThread() override;

	virtual bool checkReceiverIsReady() override;
	virtual bool isReadyToSend() override;

	virtual void sendMessageInternal(const String& message, var) override;
	virtual void sendBytesInternal(Array<uint8> data, var) override;

	virtual Array<uint8> readBytes() override;

	virtual void clearInternal() override;

	void newConnection(StreamingSocket* s) override;
	void connectionRemoved(StreamingSocket * s) override;
	void receiverBindChanged(bool isBound) override;

	ModuleUI* getModuleUI() override;

	static TCPServerModule* create() { return new TCPServerModule(); }
	virtual String getDefaultTypeString() const override { return "TCP Server"; }
};