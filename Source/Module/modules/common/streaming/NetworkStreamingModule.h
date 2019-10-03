/*
  ==============================================================================

    NetworkStreamingModule.h
    Created: 5 Jan 2018 10:42:43am
    Author:  Ben

  ==============================================================================
*/

#pragma once

#include "StreamingModule.h"

class NetworkStreamingModule :
	public StreamingModule,
	public Thread

{
public:
	NetworkStreamingModule(const String &name = "StreamingModule", bool canHaveInput = true, bool canHaveOutput = true, int defaultLocalPort = 5000, int defaultRemotePort = 5001);
	virtual ~NetworkStreamingModule();

	//RECEIVE
	std::unique_ptr<EnablingControllableContainer> receiveCC;
	IntParameter * localPort;
	BoolParameter * receiverIsBound;
	IntParameter* receiveFrequency;

	//SEND
	std::unique_ptr<EnablingControllableContainer> sendCC;
	BoolParameter * useLocal;
	StringParameter * remoteHost;
	IntParameter * remotePort;
	BoolParameter * senderIsConnected;

	virtual void setupReceiver() {}
	virtual void setupSender() {}

	virtual Array<uint8> readBytes() { return Array<uint8>(); }
	virtual bool checkReceiverIsReady() { return false; }

	virtual void clearThread();
	virtual void clearInternal() {}

	virtual void onContainerParameterChangedInternal(Parameter* p) override;
	virtual void controllableFeedbackUpdate(ControllableContainer *, Controllable * c) override;

	virtual void loadJSONDataInternal(var data) override;

	virtual void initThread() {}
	virtual void run() override;
	
	virtual String getDefaultTypeString() const override { return "NetworkStreaming"; }
};